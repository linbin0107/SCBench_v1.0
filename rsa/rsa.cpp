/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  rsa.cpp -- An implementation of the RSA public-key cipher. The
             following implementation is based on the one given in Cormen et
             al., Inroduction to Algorithms, 1991. I'll refer to this book as
             CLR because of its authors. This implementation shows the usage of
             arbitrary precision types of SystemC. That is, these types in
             SystemC can be used to implement algorithmic examples regarding
             arbitrary precision integers. The algorithms used are not the most
             efficient ones; however, they are intended for explanatory
             purposes, so they are simple and perform their job correctly.
             Below, NBITS shows the maximum number of bits in n, the variable
             that is a part of both the public and secret keys, P and S,
             respectively. NBITS can be made larger at the expense of longer
             running time. For example, CLR mentions that the RSA cipher uses
             large primes that contain approximately 100 decimal digits. This
             means that NBITS should be set to approximately 560.

             Some background knowledge: A prime number p > 1 is an integer that
             has only two divisiors, 1 and p itself. For example, 2, 3, 5, 7,
             and 11 are all primes. If p is not a prime number, it is called a
             composite number. If we are given two primes p and q, it is easy
             to find their product p * q; however, if we are given a number m
             which happens to be the product of two primes p and q that we do
             not know, it is very difficult to find p and q if m is very large,
             i.e., it is very difficult to factor m. The RSA public-key
             cryptosystem is based on this fact. Internally, we use the
             Miller-Rabin randomized primality test to deal with primes. More
             information can be obtained from pp. 831-836 in CLR, the first
             edition.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  Bin Lin, Portland State University, 3/24/2016
  Modification: add an assertion after finding two large primes p and q.

 *****************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>    // drand48, srand48
#include "rsa.h"

#define DEBUG_SYSTEMC // #undef this to disable assertions.

// Return the absolute value of x.
bigint rsa::abs_val( const sc_signed& x )
{
  if(x < 0)
    return -x;//LCOV_EXCL_BR_LINE
  else
    return x;
}

// Initialize the random number generator. If seed == -1, the
// generator will be initialized with the system time. If not, it will
// be initialized with the given seed. This way, an experiment with
// random numbers becomes reproducible.
long rsa::randomize( int seed  )
{
  long in_seed;  // time_t is long.

  if(seed <= 0)
    in_seed = time( 0 );//LCOV_EXCL_BR_LINE
  else
    in_seed = seed;

#ifndef WIN32
  srand48( in_seed );
#else
  srand( ( unsigned ) in_seed );
#endif

  return in_seed;
}

// Flip a coin with probability p.
#ifndef WIN32
bool rsa::flip( double p )
{
  return ( drand48() < p );
}
#else
bool rsa::flip( double p )
{
  const int MAX_VAL = ( 1 << 15 );

  // rand() produces an integer between 0 and 2^15-1, so rand() /
  // MAX_VAL is a number between 0 and 1, which is required to compare
  // with p.
  return ( rand() < ( int ) ( p * MAX_VAL ) );
}
#endif

// Randomly generate a bit string with nbits bits.  str has a length
// of nbits + 1. This function is used to generate random messages to
// process.
void rsa::rand_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = ( flip( 0.5 ) == true ? '1' : '0' );

  str[ nbits ] = '\0';
}

// Generate "111..111" with nbits bits for masking.
// str has a length of nbits + 1.
void rsa::max_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = '1';

  str[ nbits ] = '\0';
}

// Return a positive remainder.
bigint rsa::ret_pos( const bigint& x, const bigint& n )
{
  if ( x < 0 )
    return x + n;//LCOV_EXCL_BR_LINE
  return x;
}

// Compute the greatest common divisor ( gcd ) of a and b. This is
// Euclid's algorithm. This algorithm is at least 2,300 years old! The
// non-recursive version of this algorithm is not as elegant.
bigint rsa::gcd( const bigint& a, const bigint& b )
{
  if ( b == 0 )
    return a;
  return gcd( b, a % b );//LCOV_EXCL_BR_LINE
}

// Compute d, x, and y such that d = gcd( a, b ) = ax + by. x and y can
// be zero or negative. This algorithm is also Euclid's algorithm but
// it is extended to also find x and y. Recall that the existence of x
// and y is guaranteed by Euclid's algorithm.
void rsa::euclid( const bigint& a, const bigint& b, bigint& d, bigint& x, bigint& y )
{
  if ( b != 0 ) {
    //LCOV_EXCL_BR_START
    euclid( b, a % b, d, x, y );

    bigint tmp = x;
    x = y;
    y = tmp - ( a / b ) * y;
    //LCOV_EXCL_BR_STOP
  }
  else {
    d = a;
    x = 1;
    y = 0;
  }
}

// Return d = a^b % n, where ^ represents exponentiation.
bigint rsa::modular_exp( const bigint& a, const bigint& b, const bigint& n )
{
  bigint d = 1;//LCOV_EXCL_BR_LINE

  for ( int i = b.length() - 1; i >= 0; --i )
  {
    d = ( d * d ) % n;//LCOV_EXCL_BR_LINE
    bool b_value = b[i];//LCOV_EXCL_BR_LINE
    if ( b_value )
      d = ( d * a ) % n;//LCOV_EXCL_BR_LINE
  }

  return ret_pos( d, n );//LCOV_EXCL_BR_LINE
}

// Return the multiplicative inverse of a, modulo n, when a and n are
// relatively prime. Recall that x is a multiplicative inverse of a,
// modulo n, if a * x = 1 ( mod n ).
bigint rsa::inverse( const bigint& a, const bigint& n )
{
  bigint d, x, y;//LCOV_EXCL_BR_LINE

  euclid( a, n, d, x, y );//LCOV_EXCL_BR_LINE
  assert( d == 1 );
  x %= n;//LCOV_EXCL_BR_LINE

  return ret_pos( x, n );//LCOV_EXCL_BR_LINE
}

// Find a small odd integer a that is relatively prime to n. I do not
// know an efficient algorithm to do that but the loop below seems to
// work; it usually iterates a few times. Recall that a is relatively
// prime to n if their only common divisor is 1, i.e., gcd( a, n ) ==
// 1.
bigint rsa::find_rel_prime( const bigint& n )
{
  bigint a = 3;//LCOV_EXCL_BR_LINE
  bigint result;//LCOV_EXCL_BR_LINE
  while ( true ) {
    result = gcd(a, n);//LCOV_EXCL_BR_LINE
    if ( result == 1 )
      break;
    a += 2;//LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    bool less = (a < n);//LCOV_EXCL_BR_LINE
    assert( less );
#endif
  }

  return a;
}

// Return true if and only if a is a witness to the compositeness of
// n, i.e., a can be used to prove that n is composite.
bool rsa::witness( const bigint& a, const bigint& n )
{
  //LCOV_EXCL_BR_START
  bigint n_minus1 = n - 1;
  bigint x;
  bigint d = 1;
  //LCOV_EXCL_BR_STOP

  // Compute d = a^( n-1 ) % n.
  for ( int i = n.length() - 1; i >= 0; --i )
  {
    // Sun's SC5 bug when compiling optimized version
    // makes the wrong assignment if abs_val() is inlined
    //x = (sc_signed)d<0?-(sc_signed)d:(sc_signed)d;//abs_val( d );
    if(d<0)
      {
      x = -d;//LCOV_EXCL_BR_LINE
      assert(x==-d);
      }
    else
      {
      x = d;//LCOV_EXCL_BR_LINE
      assert(x==d);
      }
    d = ( d * d ) % n;//LCOV_EXCL_BR_LINE

    // x is a nontrivial square root of 1 modulo n ==> n is composite.
    bool result = ( abs_val( d ) == 1 ) && ( x != 1 ) && ( x != n_minus1 );//LCOV_EXCL_BR_LINE
    if ( result )
      return true;

    bool value = n_minus1[i];//LCOV_EXCL_BR_LINE
    if (value)
      d = ( d * a ) % n;//LCOV_EXCL_BR_LINE
  }

  // d = a^( n-1 ) % n != 1 ==> n is composite.
  bool val = (abs_val( d ) != 1);//LCOV_EXCL_BR_LINE
  if (val)
    return true;

  return false;
}

// Check to see if n has any small divisors. For small numbers, we do
// not have to run the Miller-Rabin primality test. We define "small"
// to be less than 1023. You can change it if necessary.
bool rsa::div_test( const bigint& n )
{
  int limit;

  if ( n < 1023 )
    limit = n.to_int() - 2;
  else
    limit = 1023;

  for ( int i = 3; i <= limit; i += 2 ) {
    if ( n % i == 0 )
      return false;   // n is composite.
  }

  return true;  // n may be prime.
}

// Return true if n is almost surely prime, return false if n is
// definitely composite.  This test, called the Miller-Rabin primality
// test, errs with probaility at most 2^(-s). CLR suggests s = 50 for
// any imaginable application, and s = 3 if we are trying to find
// large primes by applying miller_rabin to randomly chosen large
// integers. Even though we are doing the latter here, we will still
// choose s = 50. The probability of failure is at most
// 0.00000000000000088817, a pretty small number.
bool rsa::miller_rabin( const bigint& n )
{
  if ( n <= 2 )
    return false;

  bool divisor = div_test(n);//LCOV_EXCL_BR_LINE
  if ( ! divisor )
    return false;

  char str[ STR_SIZE + 1 ];

  int s = 50;
  for ( int j = 1; j <= s; ++j ) {

    // Choose a random number.
    rand_bitstr( str, STR_SIZE );

    // Set a to the chosen number.
    bigint a = str;//LCOV_EXCL_BR_LINE

    // Make sure that a is in [ 1, n - 1 ].
    a = ( a % ( n - 1 ) ) + 1;//LCOV_EXCL_BR_LINE

    // Check to see if a is a witness.
    bool wit = witness( a, n );//LCOV_EXCL_BR_LINE
    if (wit)
      return false;  // n is definitely composite.
  }

  return true;   // n is almost surely prime.
}

// Return a randomly generated, large prime number using the
// Miller-Rabin primality test.
bigint rsa::find_prime( const bigint& r )
{
  //LCOV_EXCL_BR_START
  char p_str[ HALF_STR_SIZE + 1 ];

  rand_bitstr( p_str, HALF_STR_SIZE );
  p_str[ HALF_STR_SIZE - 1 ] = '1';  // Force p to be an odd number.

  bigint p = p_str;
  //LCOV_EXCL_BR_STOP

#ifdef DEBUG_SYSTEMC
  bool res = (p > 0) && (p % 2 == 1);//LCOV_EXCL_BR_LINE
  assert( res );
#endif

  // p is randomly determined. Now, we'll look for a prime in the
  // vicinity of p. By the prime number theorem, executing the
  // following loop approximately ln ( 2^NBITS ) iterations should
  // find a prime.

#ifdef DEBUG_SYSTEMC
  // A very large counter to check against infinite loops.
  sc_bigint<NBITS> niter = 0;
#endif

  while ( ! miller_rabin( p ) ) {
    p = ( p + 2 ) % r;//LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    assert( ++niter > 0 );
#endif
  }

  return p;
}

// Encode or cipher the message in msg using the RSA public key P=( e, n ).
bigint rsa::cipher( const bigint& msg, const bigint& e, const bigint& n )
{
  return modular_exp( msg, e, n );
}

// Dencode or decipher the message in msg using the RSA secret key S=( d, n ).
bigint rsa::decipher( const bigint& msg, const bigint& d, const bigint& n )
{
  return modular_exp( msg, d, n );
}

// The RSA cipher.
void rsa::run()
{
  uint32_t nbits;
  int seed;

  ready.write(false);
  wait();

  while (true)
  {
    while (load.read() != true)
      wait();

    //LCOV_EXCL_BR_START
    nbits = num_bits.read();
    HALF_NBITS = nbits / 2;
    STR_SIZE = nbits + 2;
    HALF_STR_SIZE = HALF_NBITS + 2;

    // Generate all 1's in r.
    char r_str[ HALF_STR_SIZE + 1 ];

    seed = in.read();

    max_bitstr( r_str, HALF_STR_SIZE );
    bigint r = r_str;
    //LCOV_EXCL_BR_STOP

#ifdef DEBUG_SYSTEMC
    assert( r > 0 );
#endif

    // Initialize the random number generator.
    //LCOV_EXCL_BR_START
    cout << "\nRandom number generator seed = " << randomize( seed ) << endl;
    cout << endl;

    // Find two large primes p and q.
    bigint p = find_prime( r );
    bigint q = find_prime( r );
    // The following line was added by Bin Lin
    assert(p != q);
    //LCOV_EXCL_BR_STOP

#ifdef DEBUG_SYSTEMC
    bool positive =  ( p > 0 ) && ( q > 0 );//LCOV_EXCL_BR_LINE
    assert( positive);
#endif

    // Compute n and ( p - 1 ) * ( q - 1 ) = m.
    bigint n = p * q;//LCOV_EXCL_BR_LINE
    bigint m = ( p - 1 ) * ( q - 1 );//LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    bool nm = ( n > 0 ) && ( m > 0 );//LCOV_EXCL_BR_LINE
    assert(nm);
#endif
    // Find a small odd integer e that is relatively prime to m.
    bigint e = find_rel_prime( m );//LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    assert( e > 0 );
#endif

    // Find the multiplicative inverse d of e, modulo m.
    bigint d = inverse( e, m );//LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    assert( d > 0 );
#endif

    // Output public and secret keys.
    //LCOV_EXCL_BR_START
    cout << "RSA public key P: P=( e, n )" << endl;
    cout << "e = " << e << endl;
    cout << "n = " << n << endl;
    cout << endl;

    cout << "RSA secret key S: S=( d, n )" << endl;
    cout << "d = " << d << endl;
    cout << "n = " << n << endl;
    cout << endl;

    // Cipher and decipher a randomly generated message msg.
    //char msg_str[ STR_SIZE + 1 ];
    //rand_bitstr( msg_str, STR_SIZE );
    //bigint msg = msg_str;
    bigint msg = msg_receive.read();
    //LCOV_EXCL_BR_STOP

    // Make sure msg is smaller than n. If larger, this part
    // will be a block of the input message.
    msg %= n; //LCOV_EXCL_BR_LINE

#ifdef DEBUG_SYSTEMC
    assert( msg > 0 );
#endif
    //LCOV_EXCL_BR_START
    cout << "Message to be ciphered = " << endl;
    cout << msg << endl;

    bigint msg2 = cipher( msg, e, n );
    cout << "\nCiphered message = " << endl;
    cout << msg2 << endl;

    msg2 = decipher( msg2, d, n );
    cout << "\nDeciphered message = " << endl;
    cout << msg2 << endl;

    ready.write(true);
    msg_ciphered.write(msg);
    msg_deciphered.write(msg2);
    wait();
    //LCOV_EXCL_BR_STOP
  }
}//LCOV_EXCL_BR_LINE

