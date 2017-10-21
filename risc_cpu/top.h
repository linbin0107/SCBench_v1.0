
#ifndef _TOP_H_
#define _TOP_H_

#include "define.h"
#include "systemc.h"

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

// LCOV_EXCL_BR_START
SC_MODULE(risc_cpu) {
  sc_in<bool> clk;
//  sc_in<sc_uint<32> > addr_in;
//  sc_in<sc_uint<32> > imem_in[BOOT_LENGTH];
//  sc_in<sc_uint<32> > icmem_in[MAX_CODE_LENGTH];
  sc_in<bool> irq0;
  sc_in<bool> irq1;
  sc_in<bool> irq2;
  sc_in<bool> irq3;

 static unsigned delay_cycles;
  sc_signal<bool> intreq;

  sc_signal<bool>           icache_cs;
  sc_signal<sc_uint<32> >   icache_addr;
  sc_signal<sc_uint<32> >   icache_data;
  sc_signal<bool>           icache_valid;

  sc_signal<bool>           bios_cs;
  sc_signal<sc_uint<32> >   bios_addr;
  sc_signal<bool>           bios_valid;
  sc_signal<sc_uint<32> >   bios_data;

  sc_signal<bool>           paging_cs;
  sc_signal<sc_uint<32> >   logical_address;
  sc_signal<sc_uint<32> >   physical_address;
  sc_signal<sc_uint<32> >   paging_dout;
  sc_signal<bool>           paging_valid;


  sc_signal<sc_uint<32> >   branch_target_address;
  sc_signal<bool>           next_pc;
  sc_signal<bool>           branch_valid;
  sc_signal<sc_uint<32> >   instruction;
  sc_signal<bool>           instruction_valid;
  sc_signal<sc_uint<32> >   program_counter;
  sc_signal<bool>           branch_clear;
  sc_signal<bool>           reset;

  sc_signal<sc_int<32> >     dram_datain;
  sc_signal<bool>            dram_cs;
  sc_signal<bool>            dram_we;
  sc_signal<sc_uint<32> >    dram_addr;
  sc_signal<sc_int<32> >     dram_dout;
  sc_signal<bool>            dram_valid;
  sc_signal<bool>            dram_wr_ack;

  sc_signal<bool>             alu_cs;
  sc_signal<sc_uint<8> >      alu_op;
  sc_signal<sc_int<32> >      src_A;
  sc_signal<sc_int<32> >      src_B;
  sc_signal<sc_int<32> >      alu_dout;
  sc_signal<bool>             alu_valid;

  sc_signal<bool>             pid_valid;
  sc_signal<sc_int<32> >      pid_data;

  sc_signal<bool>   		c;
  sc_signal<bool>   		v;
  sc_signal<bool>   		z;

  sc_signal<sc_int<32> > fpu_dout;
  sc_signal<bool>        fpu_cs;
  sc_signal<bool>        fpu_valid;

//  sc_signal<bool> 		ireq0;
//  sc_signal<bool> 		ireq1;
//  sc_signal<bool> 		ireq2;
//  sc_signal<bool> 		ireq3;
  sc_signal<bool> 		rd_wr;
//  sc_signal<bool> 		intreq;
  sc_signal<sc_uint<32> > 		vectno;
  sc_signal<bool> 		intack;
  sc_signal<bool> 		intack_cpu;

  sc_signal<sc_int<32> > mmx_dout;
  sc_signal<bool>        mmx_cs;
  sc_signal<bool>        mmx_valid;

  sc_signal<bool> rfu_rst;
  sc_signal<bool> rfu_cs;
  sc_signal<bool> rfu_re;
  sc_signal<bool> rfu_we;
  sc_signal<sc_uint<4> > rfu_addr1;
  sc_signal<sc_uint<4> > rfu_addr2;
  sc_signal<sc_uint<4> > rfu_addr3;
  sc_signal<sc_int<32> > rfu_data1;
  sc_signal<sc_int<32> > rfu_data2;
  sc_signal<sc_int<32> > rfu_data3;

  // ************************ DSP *****************************************
/*  sc_signal<sc_int<32> > 		dsp_in1;
  sc_signal<sc_int<32> > 		dsp_out1;
  sc_signal<bool> 		dsp_data_valid;
  sc_signal<bool> 		dsp_input_valid;
  sc_signal<bool> 		dsp_data_requested;
*/
  fetch 	IFU;
  control CU;
  registerfile RFU;
  exec 		IEU;
  floating 	FPU;
  mmxu	 	MMXU;
  bios 		BIOS;
  paging 	PAGING;
  icache 	ICACHE;
  dcache 	DCACHE;
  pic		APIC;

  SC_CTOR(risc_cpu)
  : IFU("FETCH_BLOCK")
  , CU("CONTROL_UNIT")
  , RFU("RFU")
  , IEU("EXEC_BLOCK")
  , FPU("FLOAT_BLOCK")
  , MMXU("MMX_BLOCK")
  , BIOS("BIOS_BLOCK")
  , PAGING("PAGING_BLOCK")
  , ICACHE("ICACHE_BLOCK")
  , DCACHE("DCACHE_BLOCK")
  , APIC("PIC_BLOCK")
  {
//    IFU.addr_in(addr_in);
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
//    CU.display_done(dsp_data_valid);
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

    FPU.cs(fpu_cs);
    FPU.opcode(alu_op);
    FPU.dina(src_A);
    FPU.dinb(src_B);
    FPU.dout(fpu_dout);
    FPU.out_valid(fpu_valid);
    FPU.CLK(clk);

    MMXU.cs(mmx_cs);
    MMXU.opcode(alu_op);
    MMXU.dina(src_A);
    MMXU.dinb(src_B);
    MMXU.dout(mmx_dout);
    MMXU.out_valid(mmx_valid);
    MMXU.CLK(clk);

    BIOS.cs(bios_cs);
    BIOS.addr(physical_address);
    BIOS.dataout(bios_data);
    BIOS.bios_valid(bios_valid);
    BIOS.CLK(clk);
//    for(int i = 0; i < BOOT_LENGTH; i++)
//      BIOS.imemory_in[i](imem_in[i]);

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

    ICACHE.init_param(delay_cycles);
    ICACHE.cs(icache_cs);
    ICACHE.addr(physical_address);
    ICACHE.ld_valid(pid_valid);
    ICACHE.ld_data(pid_data);
    ICACHE.dataout(icache_data);
    ICACHE.icache_valid(icache_valid);
    ICACHE.CLK(clk);
//    for(int i = 0; i < MAX_CODE_LENGTH; i++)
//      ICACHE.icmemory_in[i](icmem_in[i]);

    DCACHE.init_param(delay_cycles);
    DCACHE.datain(dram_datain);
    DCACHE.cs(dram_cs);
    DCACHE.we(dram_we);
    DCACHE.addr(dram_addr);
    DCACHE.dout(dram_dout);
    DCACHE.out_valid(dram_valid);
    DCACHE.wr_ack(dram_wr_ack);
    DCACHE.CLK(clk);

    APIC.ireq0(irq0);
    APIC.ireq1(irq1);
    APIC.ireq2(irq2);
    APIC.ireq3(irq3);
    APIC.cs(intack_cpu);
    APIC.rd_wr(rd_wr);
    APIC.intack_cpu(intack_cpu);
    APIC.intreq(intreq);
    APIC.intack(intack);
    APIC.vectno(vectno);
  }

};
// LCOV_EXCL_BR_STOP

#endif
