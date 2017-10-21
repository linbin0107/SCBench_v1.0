
#ifndef _RISC_EXEC_H_
#define _RISC_EXEC_H_

#include "systemc.h"

// LCOV_EXCL_BR_START
struct risc_exec : sc_module {
  sc_in_clk CLK;
  sc_in<bool> reset;// reset not used.
  sc_in<bool> cs;// chip select
  sc_in<sc_uint<8> > opcode;// opcode from ID
  sc_in<sc_int<32> > dina;// operand A
  sc_in<sc_int<32> > dinb;// operand B
  sc_out<bool> C;// Carry bit
  sc_out<bool> V;// Overflow bit
  sc_out<bool> Z;// Zero bit
  sc_out<sc_int<32> > dout;// output data
  sc_out<bool> out_valid;// output valid
  sc_out<bool> isBusy;

  SC_CTOR(risc_exec) {
      SC_CTHREAD(entry, CLK.pos());
  }

  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
