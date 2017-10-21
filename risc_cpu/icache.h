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

  icache.h -- Instruction Cache Unit.

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

#ifndef _ICACHE_H_
#define _ICACHE_H_

#include "define.h"
#include "systemc.h"

// LCOV_EXCL_BR_START
SC_MODULE(icache) {
  sc_in<bool> cs;// chip select
  sc_in<sc_uint<32> > addr;// address
  sc_in<bool> ld_valid;// load valid
  sc_in<sc_int<32> > ld_data;// load data value
  sc_out<sc_uint<32> >  dataout;// ram data out
  sc_out<bool> icache_valid; // output valid
  sc_in_clk	CLK;
  // Parameter
  // icache data memory
  static sc_uint<32> icmemory[MAX_CODE_LENGTH];

  sc_uint<32> pid;// process ID

  // Number of cycles it takes to access imemory
  int wait_cycles;

  void init_param(int given_delay_cycles) {
    wait_cycles = given_delay_cycles;
  }

  //Constructor
  SC_CTOR(icache) {
    SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
