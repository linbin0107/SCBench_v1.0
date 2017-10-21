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

  paging.h -- Instruction Paging Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/
 /*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: various code updates

 *****************************************************************************/

#ifndef _PAGING_H_
#define _PAGING_H_

#include "systemc.h"

// LCOV_EXCL_BR_START
SC_MODULE(paging) {
  sc_in<bool> csin;// chip select
  sc_in<sc_uint<32> > logical_address; 	// logical address

  sc_in<sc_uint<32> > icache_din;// data from icache
  sc_in<bool> icache_validin;// data valid bit

  sc_in<sc_uint<32> > bios_din;// data from bios
  sc_in<bool> bios_validin;// data valid bit

  sc_out<sc_uint<32> > dout;// output data
  sc_out<bool> icache_csout;// output cs to cache
  sc_out<bool> bios_csout;// output cs to cache
  sc_out<sc_uint<32> > physical_address;// physical address
  sc_out<bool> data_valid;// data valid
  sc_in_clk CLK;

  signed int pid_reg;//CPU process ID register

  SC_CTOR(paging) {
    SC_CTHREAD(entry, CLK.pos());
    pid_reg = 0;
  }

  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
