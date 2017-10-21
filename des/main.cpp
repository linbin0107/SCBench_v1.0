#include <systemc.h>
#include <iostream>
#include "des.h"
#include "tb_des.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char* argv[])
{
  sc_signal < bool > reset;
  sc_signal < bool > load;
  sc_signal < bool > decrypt;
  sc_signal < sc_uint < 64 > > a;
  sc_signal < sc_uint < 64 > >key;

  sc_signal < sc_uint < 64 > > b;
  sc_signal < bool > ready;

  sc_clock clk("clk", 10, SC_NS);

  des d("des");
  d.clk(clk);
  d.reset(reset);
  d.load_i(load);
  d.decrypt_i(decrypt);
  d.data_i(a);
  d.key_i(key);
  d.data_o(b);
  d.ready_o(ready);

  testert test("tester");
  test.clk(clk);
  test.reset(reset);
  test.load(load);
  test.decrypt(decrypt);
  test.data_o(a);
  test.key(key);
  test.data_i(b);
  test.ready(ready);

#ifdef WAVE_DUMP
  sc_trace_file* trace_file = sc_create_vcd_trace_file("des");
  sc_trace<sc_uint<64> >(trace_file, a, "a");
  sc_trace<sc_uint<64> >(trace_file, key, "key");
  sc_trace<sc_uint<64> >(trace_file, b, "b");
#endif

  sc_start(1000, SC_NS);

  test.compare_results();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
