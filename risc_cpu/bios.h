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

  bios.h -- System Bios Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: major revision

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef _BIOS_H_
#define _BIOS_H_

#include "systemc.h"
#include "define.h"

//sc_uint<32> imemory[BOOT_LENGTH];

// LCOV_EXCL_BR_START
SC_MODULE(bios) {
  	sc_in<bool>	cs;// chip select
  	sc_in<sc_uint<32> > addr;// physical address
  	sc_out<bool> bios_valid;// out valid
  	sc_out<sc_uint<32> > dataout;// instruction out
  	sc_in_clk CLK;

  static sc_uint<32>  imemory[BOOT_LENGTH];	// BIOS program data memory
  sc_int<32> wait_cycles; // Cycle # it takes to access memory

  void init_param(sc_int<32> given_delay_cycles) {
    wait_cycles = given_delay_cycles;
  }

  //Constructor
  SC_CTOR(bios)
  {
    wait_cycles = 2;
    SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
