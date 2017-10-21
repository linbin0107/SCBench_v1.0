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

  floating.h -- Floating Execution Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: many code updates

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef _FLOATING_H_
#define _FLOATING_H_

#include "systemc.h"

// LCOV_EXCL_BR_START
SC_MODULE(floating) {
  sc_in<bool> cs; // chip select
  sc_in<sc_uint<8> > opcode;// opcode
  sc_in<sc_int<32> > dina;// operand A
  sc_in<sc_int<32> > dinb;// operand B
  sc_out<sc_int<32> > dout;// FPU output
  sc_out<bool> out_valid;// output valid
  sc_in_clk CLK;

  SC_CTOR(floating) {
      SC_CTHREAD(entry, CLK.pos());
  }

  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
