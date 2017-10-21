#include "rsa.h"
#include "tb_rsa.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main( int argc, char *argv[] ){
  sc_signal<int> seed;
  sc_signal<sc_uint<32> > nbits;
  sc_signal<bool> load;
  sc_signal<bool> ready;
  sc_signal<sc_bigint<250> > message;
  sc_signal<sc_bigint<250> > ciphered;
  sc_signal<sc_bigint<250> > deciphered;

  sc_clock clk("clk", 10, SC_NS);

  tb_rsa U_tb("tb");
  U_tb.clk(clk);
  U_tb.load(load);
  U_tb.ready(ready);
  U_tb.msg_send(message);
  U_tb.msg_ciphered(ciphered);
  U_tb.msg_deciphered(deciphered);
  U_tb.seed_out(seed);
  U_tb.num_bits(nbits);

  rsa U_rsa("rsa");
  U_rsa.clk(clk);
  U_rsa.in(seed);
  U_rsa.num_bits(nbits);
  U_rsa.load(load);
  U_rsa.ready(ready);
  U_rsa.msg_receive(message);
  U_rsa.msg_ciphered(ciphered);
  U_rsa.msg_deciphered(deciphered);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("rsa");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, seed, "seed");
  sc_trace(trace_file, nbits, "nbits");
  sc_trace(trace_file, load, "load");
  sc_trace(trace_file, ready, "ready");
  sc_trace(trace_file, message, "message");
  sc_trace(trace_file, ciphered, "ciphered");
  sc_trace(trace_file, deciphered, "deciphered");
#endif

  sc_start();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
