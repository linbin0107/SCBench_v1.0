

#include "risc_mmxu.h"
#include "tb_mmxu.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char** argv)
{
  sc_clock clk("clk", 10, SC_NS);

  sc_signal<bool> reset;// reset not used.
  sc_signal<bool> cs;// chip select
  sc_signal<sc_uint<8> > opcode; // opcode from ID
  sc_signal<sc_int<32> > dina; // operand A
  sc_signal<sc_int<32> > dinb; // operand B
  sc_signal<sc_int<32> > dout; // output data
  sc_signal<bool> out_valid; // output valid
  sc_signal<bool> busy;

  risc_mmxu u_mmxu("risc_mmxu");
  tb_mmxu u_test("test_mmxu");

  u_mmxu.CLK(clk);
  u_mmxu.cs(cs);
  u_mmxu.opcode(opcode);
  u_mmxu.dina(dina);
  u_mmxu.dinb(dinb);
  u_mmxu.dout(dout);
  u_mmxu.out_valid(out_valid);
  u_mmxu.isBusy(busy);

  // connect to test bench
  u_test.clk( clk );
  u_test.cs_out(cs);
  u_test.opcode_out(opcode);
  u_test.dina_out(dina);
  u_test.dinb_out(dinb);
  u_test.result(dout);
  u_test.isValid(out_valid);
  u_test.isBusy(busy);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("risc_mmxu");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, reset, "reset");
  sc_trace(trace_file, cs, "cs");
  sc_trace(trace_file, opcode, "opcode");
  sc_trace(trace_file, dina, "dina");
  sc_trace(trace_file, dinb, "dinb");
  sc_trace(trace_file, dout, "dout");
  sc_trace(trace_file, out_valid, "out_valid");
  sc_trace(trace_file, busy, "busy");
#endif

  sc_start(200, SC_NS);

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;

};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
