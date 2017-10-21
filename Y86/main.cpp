#include "systemc.h"
#include <stdlib.h>
#include <iostream>
#include "Y86.h"
#include "tb_Y86.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_STOP
int sc_main(int argc, char *argv[]) {
  sc_signal<bool> reset;
  sc_signal<sc_uint<32> > bus;
  sc_signal<sc_uint<32> > bus_A;
  sc_signal<sc_uint<32> > bus_out;
  sc_signal<bool> bus_WE;
  sc_signal<bool> bus_RE;
  sc_signal<sc_uint<8> > current_opcode;

  sc_clock clk("clk", 10, SC_NS);

  cpu_Y86 ins("ins");
  ins.clk_(clk);
  ins.rst_(reset);
  ins.bus_A_(bus_A);
  ins.bus_in_(bus);
  ins.bus_out_(bus_out);
  ins.bus_WE_(bus_WE);
  ins.bus_RE_(bus_RE);
  ins.current_opcode_(current_opcode);

  testt tt("test");
  tt.clk(clk);
  tt.reset(reset);
  tt.bus_out(bus);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("USB");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, reset, "reset");
  sc_trace(trace_file, bus, "bus");
  sc_trace(trace_file, bus_A, "bus_A");
  sc_trace(trace_file, bus_out, "bus_out");
  sc_trace(trace_file, bus_WE, "bus_WE");
  sc_trace(trace_file, bus_RE, "bus_RE");
  sc_trace(trace_file, current_opcode, "current_opcode");
#endif

  sc_start();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
