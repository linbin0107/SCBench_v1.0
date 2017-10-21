/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  exec.h -- Integer Execution Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef _EXEC_H_
#define _EXEC_H_

// LCOV_EXCL_BR_START
struct exec : sc_module {
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
  sc_in_clk CLK;

  SC_CTOR(exec) {
      SC_CTHREAD(entry, CLK.pos());
  }

  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
