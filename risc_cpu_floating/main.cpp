/***************************************************
 * File: main.cpp
 * Description: testbench file for floating
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 09/01/2016
 **************************************************/

#include "systemc.h"

#include "floating.h"
#include "tb_floating.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[])
{
  sc_signal<bool> cs;
  sc_signal<bool> valid;
  sc_signal<sc_int<32> > result;
  sc_signal<sc_uint<8> > opcode;
  sc_signal<sc_int<32> > dina;
  sc_signal<sc_int<32> > dinb;

  sc_clock clk("clk", 10, SC_NS);

  tb_floating u_tb("TB_FLOATING");
  u_tb.CLK(clk);
  u_tb.result(result);
  u_tb.isValid(valid);
  u_tb.cs(cs);
  u_tb.opcode(opcode);
  u_tb.dina(dina);
  u_tb.dinb(dinb);

  floating u_floating("FLOATING");
  u_floating.CLK(clk);
  u_floating.cs(cs);
  u_floating.opcode(opcode);
  u_floating.dina(dina);
  u_floating.dinb(dinb);
  u_floating.dout(result);
  u_floating.out_valid(valid);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("risc_floating");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, cs, "cs");
  sc_trace(trace_file, valid, "valid");
  sc_trace(trace_file, result, "result");
  sc_trace(trace_file, opcode, "opcode");
  sc_trace(trace_file, dina, "dina");
  sc_trace(trace_file, dinb, "dinb");
#endif

  sc_start(250, SC_NS);

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
