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

  fetch.h -- Instruction Fetch Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef _FETCH_H_
#define _FETCH_H_

#include "define.h"
#include "systemc.h"

// LCOV_EXCL_BR_START
SC_MODULE(fetch) {
//  sc_in<sc_uint<32> > addr_in;
  sc_in<sc_uint<32> > 		ramdata;	// instruction from RAM
  sc_in<sc_uint<32> > 		branch_address; // branch target address
  sc_in<bool>  			  next_pc;   	// pc ++
  sc_in<bool>  			  branch_valid;  	// branch_valid
  sc_in<bool>  			  interrupt;   	// interrrupt
  sc_in<sc_uint<32> >  		int_vectno;   	// interrupt vector number
  sc_in<bool>  			  ram_valid;   	// ram valid
  sc_out<bool>        ram_cs;     // RAM chip select
  sc_out<sc_uint<32> >   address;      // address send to RAM
  sc_out<sc_uint<32> >  	instruction; 	 // instruction send to ID
  sc_out<bool>  		  instruction_valid; // inst valid
  sc_out<sc_uint<32> >  	program_counter; // program counter
  sc_out<bool>  		  interrupt_ack;   // interrupt acknowledge
  sc_out<bool>  		  branch_clear;    // clear outstanding branch
  sc_out<bool>  		  reset;  	// reset
  sc_in_clk           CLK;

  int memory_latency;

  //Constructor
  SC_CTOR(fetch) {
    SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();
};
// LCOV_EXCL_BR_STOP

#endif
