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

  main -- This is a simple CPU modeling using SystemC.
          Architecure defined by Martin Wang.
          You can initialize register by modifying file
          named register, and so is bios, and dacache.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: major revision

 *****************************************************************************/


#include <systemc.h>
#include "bios.h"
#include "paging.h"
#include "icache.h"
#include "fetch.h"
#include "control.h"
#include "exec.h"
#include "mmxu.h"
#include "floating.h"
#include "dcache.h"
#include "pic.h"
#include "registerfile.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int ac, char *av[])
{
  unsigned delay_cycles = 2;

  sc_signal<bool>           icache_cs("ICACHE_CS") ;
  sc_signal<sc_uint<32> >   icache_addr("ICACHE_ADDR");
  sc_signal<sc_uint<32> >   icache_data("ICACHE_DATAOUT") ;
  sc_signal<bool>           icache_valid("ICACHE_VALID") ;

  sc_signal<bool>           bios_cs("BIOS_CS") ;
  sc_signal<sc_uint<32> >   bios_addr("BIOS_ADDR");
  sc_signal<bool>           bios_valid("BIOS_VALID") ;
  sc_signal<sc_uint<32> >   bios_data("BIOS_DATAOUT") ;

  sc_signal<bool>           paging_cs("PAGING_CS");
  sc_signal<sc_uint<32> >   logical_address("LOGICAL_ADDRESS") ;
  sc_signal<sc_uint<32> >   physical_address("PHYSICAL_ADDRESS") ;
  sc_signal<sc_uint<32> >   paging_dout("PAGING_DOUT") ;
  sc_signal<bool>           paging_valid("PAGING_VALID") ;

  sc_signal<sc_uint<32> >   branch_target_address("BRANCH_TARGET_ADDRESS") ;
  sc_signal<bool>           next_pc("NEXT_PC") ;
  sc_signal<bool>           branch_valid("BRANCH_VALID") ;
  sc_signal<sc_uint<32> >   instruction("INSTRUCTION") ;
  sc_signal<bool>           instruction_valid("INSTRUCTION_VALID") ;
  sc_signal<sc_uint<32> >   program_counter("PROGRAM_COUNTER") ;
  sc_signal<bool>           branch_clear("BRANCH_CLEAR") ;
  sc_signal<bool>           reset("RESET") ;

  sc_signal<sc_int<32> >     dram_datain("DRAM_DATAIN") ;
  sc_signal<bool>            dram_cs("DRAM_CS") ;
  sc_signal<bool>            dram_we("DRAM_WE") ;
  sc_signal<sc_uint<32> >    dram_addr("DRAM_ADDR");
  sc_signal<sc_int<32> >     dram_dout("DRAM_DATAOUT") ;
  sc_signal<bool>            dram_valid("DRAM_VALID") ;
  sc_signal<bool>            dram_wr_ack("DRAM_WR_ACK") ;

  sc_signal<bool>             alu_cs("ALU_CS") ;
  sc_signal<sc_uint<8> >      alu_op("ALU_OP") ;
  sc_signal<sc_int<32> >      src_A("SRC_A") ;
  sc_signal<sc_int<32> >      src_B("SRC_B") ;
  sc_signal<sc_int<32> >      alu_dout("ALU_DOUT") ;
  sc_signal<bool>             alu_valid("ALU_VALID") ;

  sc_signal<bool>             pid_valid("PID_VALID") ;
  sc_signal<sc_int<32> >      pid_data("PID_DATA") ;

  sc_signal<bool>   		c("C") ;
  sc_signal<bool>   		v("V") ;
  sc_signal<bool>   		z("Z") ;

  sc_signal<sc_int<32> > fpu_dout("FPU_DOUT") ;
  sc_signal<bool>        fpu_cs("FPU_CS") ;
  sc_signal<bool>        fpu_valid("FPU_VALID") ;

  sc_signal<bool> 		ireq0("IREQ0") ;
  sc_signal<bool> 		ireq1("IREQ1") ;
  sc_signal<bool> 		ireq2("IREQ2") ;
  sc_signal<bool> 		ireq3("IREQ3") ;
  sc_signal<bool> 		rd_wr("RD_WR") ;
  sc_signal<bool> 		intreq("INTREQ") ;
  sc_signal<sc_uint<32> > 		vectno("VECTNO") ;
  sc_signal<bool> 		intack("INTACK") ;
  sc_signal<bool> 		intack_cpu("INTACK_CPU") ;

  sc_signal<sc_int<32> > mmx_dout("MMX_DOUT");
  sc_signal<bool>        mmx_cs("MMX_CS") ;
  sc_signal<bool>        mmx_valid("MMX_VALID") ;

  sc_signal<bool> rfu_rst("RFU_RST");
  sc_signal<bool> rfu_cs("RFU_CS");
  sc_signal<bool> rfu_re("RFU_RE");
  sc_signal<bool> rfu_we("RFU_WE");
  sc_signal<sc_uint<4> > rfu_addr1("RFU_ADDR1");
  sc_signal<sc_uint<4> > rfu_addr2("RFU_ADDR2");
  sc_signal<sc_uint<4> > rfu_addr3("RFU_ADDR3");
  sc_signal<sc_int<32> > rfu_data1("RFU_DATA1");
  sc_signal<sc_int<32> > rfu_data2("RFU_DATA2");
  sc_signal<sc_int<32> > rfu_data3("RFU_DATA3");

  // ************************ DSP *****************************************
  sc_signal<sc_int<32> > 		dsp_in1("DPS_IN1");
  sc_signal<sc_int<32> > 		dsp_out1("DSP_OUT1");
  sc_signal<bool> 		dsp_data_valid("DSP_DATA_VALID");
  sc_signal<bool> 		dsp_input_valid("DSP_INPUT_VALID");
  sc_signal<bool> 		dsp_data_requested("DSP_DATA_REQUESTED");

  ////////////////////////////////////////////////////////////////////////////
  // 				MAIN PROGRAM
  ////////////////////////////////////////////////////////////////////////////
  //sc_clock clk("Clock", 1, SC_NS, 0.5, 0.0, SC_NS);
  sc_clock clk("Clock", 10, SC_NS);

  std::cout << ("/////////////////////////////////////////////////////////////////////////\n");
  std::cout << ("//  This code is written at SYNOPSYS, Inc.\n");
  std::cout << ("/////////////////////////////////////////////////////////////////////////\n");
  std::cout << ("//  Module   : main of CPU Model\n");
  std::cout << ("//  Author   : Martin Wang\n");
  std::cout << ("//  Company  : SYNOPSYS, Inc.\n");
  std::cout << ("//  Purpose  : This is a simple CPU modeling using SystemC.\n");
  std::cout << ("//             Instruction Set Architecure defined by Martin Wang.\n");
  std::cout << ("//             \n");
  std::cout << ("//           SystemC (TM) Copyright (c) 1988-2001 by Synopsys, Inc.  \n");
  std::cout << ("//             \n");
  std::cout << ("/////////////////////////////////////////////////////////////////////////\n");
  std::cout << "// IN THIS MACHINE Integer is " << sizeof (int) << " bytes.\n";
  std::cout << "// IN THIS MACHINE Floating is " << sizeof (float) << " bytes.\n";
  std::cout << "// IN THIS MACHINE Double is " << sizeof (double) << " bytes.\n";
  std::cout << ("//     \n");
  std::cout << ("//     \n");
  std::cout << ("//                            .,,uod8B8bou,,.\n");
  std::cout << ("//                   ..,uod8BBBBBBBBBBBBBBBBRPFT?l!i:.\n");
  std::cout << ("//              ,=m8BBBBBBBBBBBBBBBRPFT?!||||||||||||||\n");
  std::cout << ("//              !...:!TVBBBRPFT||||||||||!!^^\"\"    ||||\n");
  std::cout << ("//              !.......:!?|||||!!^^\"\"'            ||||\n");
  std::cout << ("//              !.........||||        ###  #  #    ||||\n");
  std::cout << ("//              !.........||||  ###  #  #  #  #    ||||\n");
  std::cout << ("//              !.........|||| #     #  #  #  #    ||||\n");
  std::cout << ("//              !.........|||| #     # #   #  #    ||||\n");
  std::cout << ("//              !.........|||| #     ##    #  #    ||||\n");
  std::cout << ("//              !.........|||| #     #     ###     ||||\n");
  std::cout << ("//              `.........|||| #   # #            ,||||\n");
  std::cout << ("//               .;.......||||  ###          _.-!!|||||\n");
  std::cout << ("//        .,uodWBBBBb.....||||       _.-!!|||||||||!:'\n");
  std::cout << ("//     !YBBBBBBBBBBBBBBb..!|||:..-!!|||||||!iof68BBBBBb....\n");
  std::cout << ("//     !..YBBBBBBBBBBBBBBb!!||||||||!iof68BBBBBBRPFT?!::   `.\n");
  std::cout << ("//     !....YBBBBBBBBBBBBBBbaaitf68BBBBBBRPFT?!:::::::::     `.\n");
  std::cout << ("//     !......YBBBBBBBBBBBBBBBBBBBRPFT?!::::::;:!^\"`;:::       `.\n");
  std::cout << ("//     !........YBBBBBBBBBBRPFT?!::::::::::^''...::::::;         iBBbo.\n");
  std::cout << ("//     `..........YBRPFT?!::::::::::::::::::::::::;iof68bo.      WBBBBbo.\n");
  std::cout << ("//       `..........:::::::::::::::::::::::;iof688888888888b.     `YBBBP^'\n");
  std::cout << ("//         `........::88::::::::::::;iof688888888888888888888b.     `\n");
  std::cout << ("//           `......::81:::::;iof688888888888888888888888888888b.\n");
  std::cout << ("//             `....:::;iof688888888888888888888888888888888899fT!\n");
  std::cout << ("//               `..::!8888888888888888888888888888888899fT|!^\"'\n");
  std::cout << ("//                 `' !!988888888888888888888888899fT|!^\"'\n");
  std::cout << ("//                     `!!8888888888888888899fT|!^\"'\n");
  std::cout << ("//                       `!988888888899fT|!^\"'\n");
  std::cout << ("//                         `!9899fT|!^\"'\n");
  std::cout << ("//                           `!^\"'\n");
  std::cout << ("//     \n");
  std::cout << ("//     \n");
  std::cout << ("/////////////////////////////////////////////////////////////////////////\n\n\n");

  fetch IFU("FETCH_BLOCK");
  IFU.ramdata(paging_dout);
  IFU.branch_address(branch_target_address);
  IFU.next_pc(next_pc);
  IFU.branch_valid(branch_valid);
  IFU.interrupt(intreq);
  IFU.int_vectno(vectno);
  IFU.ram_valid(paging_valid);
  IFU.ram_cs(paging_cs);
  IFU.address(logical_address);
  IFU.instruction(instruction);
  IFU.instruction_valid(instruction_valid);
  IFU.program_counter(program_counter);
  IFU.interrupt_ack(intack_cpu);
  IFU.branch_clear(branch_clear);
  IFU.reset(reset);
  IFU.CLK(clk);

  control CU("CONTROL_UNIT");
  CU.resetin(reset);
  CU.instruction(instruction);
  CU.instruction_valid(instruction_valid);
  CU.alu_valid(alu_valid);
  CU.alu_dout(alu_dout);
  CU.dram_dout(dram_dout);
  CU.dram_rd_valid(dram_valid);
  CU.dram_wr_ack(dram_wr_ack);
  CU.fpu_dout(fpu_dout);
  CU.fpu_valid(fpu_valid);
  CU.mmx_dout(mmx_dout);
  CU.mmx_valid(mmx_valid);
  CU.clear_branch(branch_clear);
  //CU.display_done(dsp_data_valid);
  CU.pc(program_counter);
  CU.next_pc(next_pc);
  CU.rfu_data1(rfu_data1);
  CU.rfu_data2(rfu_data2);
  CU.branch_valid(branch_valid);
  CU.branch_target_address(branch_target_address);
  CU.dram_addr(dram_addr);
  CU.dram_datain(dram_datain);
  CU.alu_op(alu_op);
  CU.dram_we(dram_we);
  CU.src_A(src_A);
  CU.src_B(src_B);
  CU.alu_cs(alu_cs);
  CU.dram_cs(dram_cs);
  CU.fpu_cs(fpu_cs);
  CU.mmx_cs(mmx_cs);
  CU.pid_valid(pid_valid);
  CU.pid_data(pid_data);
  CU.rfu_rst(rfu_rst);
  CU.rfu_cs(rfu_cs);
  CU.rfu_re(rfu_re);
  CU.rfu_we(rfu_we);
  CU.rfu_addr1(rfu_addr1);
  CU.rfu_addr2(rfu_addr2);
  CU.rfu_addr3(rfu_addr3);
  CU.rfu_data3(rfu_data3);
  CU.CLK(clk);

  registerfile RFU("RFU");
  RFU.clk(clk);
  RFU.rst(rfu_rst);
  RFU.cs(rfu_cs);
  RFU.re(rfu_re);
  RFU.we(rfu_we);
  RFU.addr1(rfu_addr1);
  RFU.addr2(rfu_addr2);
  RFU.addr3(rfu_addr3);
  RFU.data1(rfu_data1);
  RFU.data2(rfu_data2);
  RFU.data3(rfu_data3);

  exec IEU("EXEC_BLOCK");
  IEU.reset(reset);
  IEU.cs(alu_cs);
  IEU.opcode(alu_op);
  IEU.dina(src_A);
  IEU.dinb(src_B);
  IEU.C(c);
  IEU.V(v);
  IEU.Z(z);
  IEU.dout(alu_dout);
  IEU.out_valid(alu_valid);
  IEU.CLK(clk);

  floating 	FPU("FLOAT_BLOCK");
  FPU.cs(fpu_cs);
  FPU.opcode(alu_op);
  FPU.dina(src_A);
  FPU.dinb(src_B);
  FPU.dout(fpu_dout);
  FPU.out_valid(fpu_valid);
  FPU.CLK(clk);

  mmxu MMXU("MMX_BLOCK");
  MMXU.cs(mmx_cs);
  MMXU.opcode(alu_op);
  MMXU.dina(src_A);
  MMXU.dinb(src_B);
  MMXU.dout(mmx_dout);
  MMXU.out_valid(mmx_valid);
  MMXU.CLK(clk);

  bios BIOS("BIOS_BLOCK");
  BIOS.cs(bios_cs);
  BIOS.addr(physical_address);
  BIOS.dataout(bios_data);
  BIOS.bios_valid(bios_valid);
  BIOS.CLK(clk);

  paging PAGING("PAGING_BLOCK");
  PAGING.csin(paging_cs);
  PAGING.logical_address(logical_address);
  PAGING.icache_din(icache_data);
  PAGING.icache_validin(icache_valid);
  PAGING.bios_din(bios_data);
  PAGING.bios_validin(bios_valid);
  PAGING.icache_csout(icache_cs);
  PAGING.bios_csout(bios_cs);
  PAGING.physical_address(physical_address);
  PAGING.dout(paging_dout);
  PAGING.data_valid(paging_valid);
  PAGING.CLK(clk);

  icache ICACHE("ICACHE_BLOCK");
  ICACHE.init_param(delay_cycles);
  ICACHE.cs(icache_cs);
  ICACHE.addr(physical_address);
  ICACHE.ld_valid(pid_valid);
  ICACHE.ld_data(pid_data);
  ICACHE.dataout(icache_data);
  ICACHE.icache_valid(icache_valid);
  ICACHE.CLK(clk);

  dcache DCACHE("DCACHE_BLOCK");
  DCACHE.init_param(delay_cycles);
  DCACHE.datain(dram_datain);
  DCACHE.cs(dram_cs);
  DCACHE.we(dram_we);
  DCACHE.addr(dram_addr);
  DCACHE.dout(dram_dout);
  DCACHE.out_valid(dram_valid);
  DCACHE.wr_ack(dram_wr_ack);
  DCACHE.CLK(clk);

  pic APIC("PIC_BLOCK");
  APIC.ireq0(ireq0);
  APIC.ireq1(ireq1);
  APIC.ireq2(ireq2);
  APIC.ireq3(ireq3);
  APIC.cs(intack_cpu);
  APIC.rd_wr(rd_wr);
  APIC.intack_cpu(intack_cpu);
  APIC.intreq(intreq);
  APIC.intack(intack);
  APIC.vectno(vectno);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("risc_cpu");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, icache_cs, "icache_cs");
  sc_trace(trace_file, icache_addr, "icache_addr");
  sc_trace(trace_file, icache_data, "icache_data");
  sc_trace(trace_file, icache_valid, "icache_valide");

  sc_trace(trace_file, bios_cs, "bios_cs");
  sc_trace(trace_file, bios_addr, "bios_addr");
  sc_trace(trace_file, bios_valid, "bios_valid");
  sc_trace(trace_file, bios_data, "bios_data");

  sc_trace(trace_file, paging_cs, "paging_cs");
  sc_trace(trace_file, logical_address, "logical_address");
  sc_trace(trace_file, physical_address, "physical_address");
  sc_trace(trace_file, paging_dout, "paging_dout");
  sc_trace(trace_file, paging_valid, "paging_valid");

  sc_trace(trace_file, branch_target_address, "branch_target_address");
  sc_trace(trace_file, next_pc, "next_pc");
  sc_trace(trace_file, branch_valid, "branch_valid");
  sc_trace(trace_file, instruction, "instruction");
  sc_trace(trace_file, instruction_valid, "instruction_valid");
  sc_trace(trace_file, program_counter, "program_counter");
  sc_trace(trace_file, branch_clear, "branch_clear");
  sc_trace(trace_file, reset, "reset");

  sc_trace(trace_file, dram_datain, "dram_datain");
  sc_trace(trace_file, dram_cs, "dram_cs");
  sc_trace(trace_file, dram_we, "dram_we");
  sc_trace(trace_file, dram_addr, "dram_addr");
  sc_trace(trace_file, dram_dout, "dram_dout");
  sc_trace(trace_file, dram_valid, "dram_valid");
  sc_trace(trace_file, dram_wr_ack, "dram_wr_ack");

  sc_trace(trace_file, alu_cs, "alu_cs");
  sc_trace(trace_file, alu_op, "alu_op");
  sc_trace(trace_file, src_A, "src_A");
  sc_trace(trace_file, src_B, "src_B");
  sc_trace(trace_file, alu_dout, "alu_dout");
  sc_trace(trace_file, alu_valid, "alu_valid");

  sc_trace(trace_file, pid_valid, "pid_valid");
  sc_trace(trace_file, pid_data, "pid_data");

  sc_trace(trace_file, c, "c");
  sc_trace(trace_file, v, "v");
  sc_trace(trace_file, z, "z");

  sc_trace(trace_file, fpu_dout, "fpu_dout");
  sc_trace(trace_file, fpu_cs, "fpu_cs");
  sc_trace(trace_file, fpu_valid, "fpu_valid");

  sc_trace(trace_file, ireq0, "ireq0");
  sc_trace(trace_file, ireq1, "ireq1");
  sc_trace(trace_file, ireq2, "ireq2");
  sc_trace(trace_file, ireq3, "ireq3");
  sc_trace(trace_file, rd_wr, "rd_wr");
  sc_trace(trace_file, intreq, "intreq");
  sc_trace(trace_file, vectno, "vectno");
  sc_trace(trace_file, intack, "intack");
  sc_trace(trace_file, intack_cpu, "intack_cpu");

  sc_trace(trace_file, mmx_dout, "mmx_dout");
  sc_trace(trace_file, mmx_cs, "mmx_cs");
  sc_trace(trace_file, mmx_valid, "mmx_valid");

  sc_trace(trace_file, rfu_rst, "rfu_rst");
  sc_trace(trace_file, rfu_cs, "rfu_cs");
  sc_trace(trace_file, rfu_re, "rfu_re");
  sc_trace(trace_file, rfu_we, "rfu_we");

  sc_trace(trace_file, rfu_addr1, "rfu_addr1");
  sc_trace(trace_file, rfu_addr2, "rfu_addr2");
  sc_trace(trace_file, rfu_addr3, "rfu_addr3");
  sc_trace(trace_file, rfu_data1, "rfu_data1");
  sc_trace(trace_file, rfu_data2, "rfu_data2");
  sc_trace(trace_file, rfu_data3, "rfu_data3");
#endif

  sc_start(2000, SC_US);

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;	/* this is necessary */
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
