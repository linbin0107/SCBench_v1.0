#include <stdint.h>
#include "systemc.h"

// NBITS is the number of bits in n of public and secret     keys P and
// S. HALF_NBITS is the number of bits in p and q, which     are the prime
// factors of n.
#define NBITS         250
//#define HALF_NBITS    ( NBITS / 2 )

// +2 is for the format specifier '0b' to make the strin    g binary.
//#define STR_SIZE      ( NBITS + 2 )
//#define HALF_STR_SIZE ( HALF_NBITS + 2 )

//LCOV_EXCL_BR_START
typedef sc_bigint<NBITS>  bigint;

SC_MODULE(rsa){
  sc_in_clk clk;
  sc_in<int> in;
  sc_in<bool> load;
  sc_in<sc_uint<32> > num_bits;
  sc_in<bigint> msg_receive;

  sc_out<bool> ready;
  sc_out<bigint> msg_ciphered;
  sc_out<bigint> msg_deciphered;

  uint32_t HALF_NBITS;
  uint32_t STR_SIZE;
  uint32_t HALF_STR_SIZE;

  bigint decipher(const bigint& msg, const bigint& d, const bigint& n);
  bigint cipher(const bigint& msg, const bigint& e, const bigint& n);
  bigint find_prime( const bigint& r );
  bool miller_rabin( const bigint& n );
  bool div_test( const bigint& n );
  bool witness( const bigint& a, const bigint& n );
  bigint find_rel_prime( const bigint& n);
  bigint inverse( const bigint& a, const bigint& n);
  bigint modular_exp( const bigint& a, const bigint& b, const bigint& n );
  void euclid( const bigint& a, const bigint& b, bigint& d, bigint& x, bigint& y);
  bigint gcd( const bigint& a, const bigint& b );
  bigint ret_pos( const bigint& x, const bigint& n);
  void max_bitstr( char *str, int nbits);
  void rand_bitstr( char *str, int nbits);
  bool flip( double p );
  long randomize( int seed);
  bigint abs_val( const sc_signed& x);

  void run();

  SC_CTOR( rsa ) {
    SC_THREAD( run );
    sensitive << clk.pos();
    dont_initialize();
  }
};
//LCOV_EXCL_BR_STOP
