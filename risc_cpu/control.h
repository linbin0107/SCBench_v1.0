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

  control.h -- Control Unit implements the DECODE, EXECUTE, MEM, and
               WRITEBACK stages.

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

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <systemc.h>

// LCOV_EXCL_BR_START
SC_MODULE(control) {
  	sc_in<bool> resetin;// input reset
  	sc_in<sc_uint<32> > instruction;// fetched instruction
  	sc_in<bool> instruction_valid;// input valid
  	sc_in<bool> alu_valid; // ALU data valid
  	sc_in<sc_int<32> > alu_dout; // data from ALU
  	sc_in<sc_int<32> > dram_dout; // data from Dcache
  	sc_in<bool> dram_rd_valid; // Dcache read data valid
  	sc_in<bool> dram_wr_ack; // Dcache write ack
  	sc_in<sc_int<32> > fpu_dout;// data from FPU
  	sc_in<bool> fpu_valid;// FPU data valid
  	sc_in<sc_int<32> > 	mmx_dout;// data from FPU
  	sc_in<bool> mmx_valid; // data valid from MMX
  	sc_in<bool> clear_branch;// clear outstanding branch
//  	sc_in<bool> display_done;// display to monitor done
  	sc_in<sc_uint<32> >	pc; // program counter from IFU
    sc_in<sc_int<32> > rfu_data1; // RFU data1
    sc_in<sc_int<32> > rfu_data2; // RFU data2

  	sc_out<bool> next_pc;// next pc ++ ?
  	sc_out<bool> branch_valid;// branch valid signal
  	sc_out<sc_uint<32> > branch_target_address;	// branch target address
  	sc_out<bool> dram_cs; // memory access valid
  	sc_out<sc_uint<32> > dram_addr;// memory physical address
  	sc_out<sc_int<32> > dram_datain;// memory datain
  	sc_out<sc_uint<8> > alu_op; // ALU/FPU/MMU Opcode
  	sc_out<bool> dram_we; // memory write enable
  	sc_out<sc_int<32> > src_A;// operand A
  	sc_out<sc_int<32> > src_B;// operand B
  	sc_out<bool> alu_cs;// enable ALU
  	sc_out<bool> fpu_cs;// enable FPU
  	sc_out<bool> mmx_cs;// enable MMU
  	sc_out<bool> pid_valid;// load process ID
  	sc_out<sc_int<32> > pid_data;// process ID value
    sc_out<bool> rfu_rst;
    sc_out<bool> rfu_cs;
    sc_out<bool> rfu_re;
    sc_out<bool> rfu_we;
    sc_out<sc_uint<4> > rfu_addr1;    // RFU addr1
    sc_out<sc_uint<4> > rfu_addr2;    // RFU addr2
    sc_out<sc_uint<4> > rfu_addr3;    // RFU addr3
    sc_out<sc_int<32> > rfu_data3;    // RFU data3
    sc_in_clk 			CLK;

  //Constructor
  SC_CTOR(control) {
    SC_CTHREAD(entry, CLK.pos());
  }

  // Process functionality in member function below
  void entry();

};
// LCOV_EXCL_BR_STOP

#endif
