
#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_rsa) {
  sc_in_clk clk;
  sc_in<bool> ready;
  sc_in<sc_bigint<250> > msg_ciphered;
  sc_in<sc_bigint<250> > msg_deciphered;

  sc_out<bool> load;
  sc_out<int> seed_out;
  sc_out<sc_uint<32> > num_bits;
  sc_out<sc_bigint<250> > msg_send;

  bool flip( double p );
  void rand_bitstr(char *str, int nbits);

  void driver_monitor();

  SC_CTOR(tb_rsa) {
    SC_THREAD(driver_monitor);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
