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

  dcache.h -- Data Cache Unit.

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

#ifndef _DCACHE_H_
#define _DCACHE_H_

#define DCACHE_SIZE 4096

// LCOV_EXCL_BR_START
struct dcache : sc_module {
  	sc_in<sc_int<32> > datain; // input data
  	sc_in<bool> cs; // chip select
  	sc_in<bool> we; // write enable
  	sc_in<sc_uint<32> > addr; // address
  	sc_out<sc_int<32> > dout; // data out
  	sc_out<bool> out_valid; // output valid
    sc_out<bool> wr_ack; // ack write
    sc_in_clk CLK;

  // Parameter
  static sc_int<32> dmemory[DCACHE_SIZE];// data memory
  int wait_cycles;// cycles # it takes to access dmemory

  void init_param(int given_delay_cycles) {
    wait_cycles = given_delay_cycles;
  }

  //Constructor
  SC_CTOR(dcache) {
    wait_cycles = 2;
    SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
