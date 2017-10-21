#include <stdint.h>
#include "tb_rsa.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START

// Flip a coin with probability p.
#ifndef WIN32
bool tb_rsa::flip( double p )
{
  return ( drand48() < p );
}
#else
bool tb_rsa::flip( double p )
{
  const int MAX_VAL = ( 1 << 15 );

  // rand() produces an integer between 0 and 2^15-1, so rand() /
  // MAX_VAL is a number between 0 and 1, which is required to compare
  // with p.
  return ( rand() < ( int ) ( p * MAX_VAL ) );
}
#endif

// Randomly generate a bit string with nbits bits. str has a length
// of nbits + 1. This function is used to generate random messages to
// process.
void tb_rsa::rand_bitstr( char *str, int nbits )
{
  assert( nbits >= 4 );

  str[ 0 ] = '0';
  str[ 1 ] = 'b';
  str[ 2 ] = '0';  // Sign for positive numbers.

  for ( int i = 3; i < nbits; ++i )
    str[ i ] = ( flip( 0.5 ) == true ? '1' : '0' );

  str[ nbits ] = '\0';
}

void tb_rsa::driver_monitor()
{
  int seed;
  uint32_t nbits;

  sc_bigint<250> ciphered;
  sc_bigint<250> deciphered;

  load.write(false);

  wait();

  while (true)
  {
    nbits = 250;
    seed = -1;

    char msg_str[nbits+3];
    rand_bitstr(msg_str, nbits+2);

    seed_out.write(seed);
    num_bits.write(nbits);

    msg_send.write(msg_str);

    load.write(true);
    wait();

    load.write(false);
    wait();

    while (ready.read() != true )
      wait();

    ciphered = msg_ciphered.read();
    deciphered = msg_ciphered.read();

    assert((ciphered == deciphered) && "Something wrong");

    cout << "Finished simulation SUCCESSFULLY" << endl;

    sc_stop();

    wait();
  }
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
