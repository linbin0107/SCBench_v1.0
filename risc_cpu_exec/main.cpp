
#include "risc_exec.h"
#include "tb_exec.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char** argv)
{
  sc_clock clk("clk", 10, SC_NS);

  sc_signal<bool> reset;// reset not used.
  sc_signal<bool> cs;// chip select
  sc_signal<sc_uint<8> > opcode;// opcode from ID
  sc_signal<sc_int<32> > dina;// operand A
  sc_signal<sc_int<32> > dinb;// operand B
  sc_signal<bool> C;// Carry bit
  sc_signal<bool> V;// Overflow bit
  sc_signal<bool> Z;// Zero bit
  sc_signal<sc_int<32> > dout;// output data
  sc_signal<bool> out_valid;// output valid
  sc_signal<bool> busy;

  tb_exec u_test("test_exec");
  risc_exec u_exec("risc_exec");

  u_exec.CLK(clk);
  u_exec.reset(reset);
  u_exec.cs(cs);
  u_exec.opcode(opcode);
  u_exec.dina(dina);
  u_exec.dinb(dinb);
  u_exec.C(C);
  u_exec.V(V);
  u_exec.Z(Z);
  u_exec.dout(dout);
  u_exec.out_valid(out_valid);
  u_exec.isBusy(busy);

  // connect to test bench
  u_test.clk( clk );
  u_test.cs_out(cs);
  u_test.carry(C);
  u_test.overflow(V);
  u_test.zero(Z);
  u_test.result(dout);
  u_test.isValid(out_valid);
  u_test.isBusy(busy);
  u_test.opcode_out(opcode);
  u_test.dina_out(dina);
  u_test.dinb_out(dinb);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("risc_exec");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, reset, "reset");
  sc_trace(trace_file, cs, "cs");
  sc_trace(trace_file, opcode, "opcode");
  sc_trace(trace_file, dina, "dina");
  sc_trace(trace_file, dinb, "dinb");
  sc_trace(trace_file, C, "C");
  sc_trace(trace_file, V, "V");
  sc_trace(trace_file, Z, "Z");
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
