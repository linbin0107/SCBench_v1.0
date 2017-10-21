/***************************************************
 * File: main.cpp
 * Description: testbench file for control unit
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 04/01/2015
 **************************************************/

#include "risc_control.h"
#include "tb_control.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char** argv)
{
  sc_clock clk("clk", 10, SC_NS);

  sc_signal<bool> resetin;              // input reset
  sc_signal<sc_uint<32> > instruction;        // fetched instruction
  sc_signal<bool> instruction_valid;// input valid
  sc_signal<bool> alu_valid;        // ALU data valid
  sc_signal<sc_int<32> > alu_dout;         // data from ALU
  sc_signal<sc_int<32> > dram_dout;     // data from Dcache
  sc_signal<bool> dram_rd_valid;    // Dcache read data valid
  sc_signal<bool> dram_wr_ack;      // Dcache write ack
  sc_signal<sc_int<32> > fpu_dout;           // data from FPU
  sc_signal<bool> fpu_valid;        // FPU data valid
  sc_signal<sc_int<32> > mmx_dout;           // data from FPU
  sc_signal<bool> mmx_valid;        // data valid from MMX
  sc_signal<bool> clear_branch;         // clear outstanding branch
  sc_signal<bool> display_done;         // display to monitor done
  sc_signal<sc_uint<32> > pc;             // program counter from IFU
  sc_signal<sc_int<32> > rfu_data1;       // RFU data1
  sc_signal<sc_int<32> > rfu_data2;       // RFU data2

  sc_signal<bool> next_pc;            // next pc ++ ?
  sc_signal<bool> branch_valid;       // branch valid signal
  sc_signal<sc_uint<32> > branch_target_address;  // branch target address
  sc_signal<bool> dram_cs;        // memory access valid
  sc_signal<sc_uint<32> > dram_addr;          // memory physical address
  sc_signal<sc_int<32> > dram_datain;        // memory datain
  sc_signal<sc_uint<8> > alu_op;             // ALU/FPU/MMU Opcode
  sc_signal<bool> dram_we;          // memory write enable
  sc_signal<sc_int<32> > src_A;          // operand A
  sc_signal<sc_int<32> > src_B;          // operand B
  sc_signal<bool> alu_cs;         // enable ALU
  sc_signal<bool> fpu_cs;         // enable FPU
  sc_signal<bool> mmx_cs;           // enable MMU
  sc_signal<bool> pid_valid;            // load process ID
  sc_signal<sc_int<32> > pid_data;       // process ID value
  sc_signal<bool> rfu_rst;
  sc_signal<bool> rfu_cs;
  sc_signal<bool> rfu_re;
  sc_signal<bool> rfu_we;
  sc_signal<sc_uint<4> > rfu_addr1;    // RFU addr1
  sc_signal<sc_uint<4> > rfu_addr2;    // RFU addr2
  sc_signal<sc_uint<4> > rfu_addr3;    // RFU addr3
  sc_signal<sc_int<32> > rfu_data3;    // RFU data3

  risc_control u_control("risc_control");
  tb_control u_test("test_control");

  u_control.CLK(clk);
  u_control.resetin(resetin);
  u_control.instruction(instruction);
  u_control.instruction_valid(instruction_valid);
  u_control.alu_valid(alu_valid);
  u_control.alu_dout(alu_dout);
  u_control.dram_dout(dram_dout);
  u_control.dram_rd_valid(dram_rd_valid);
  u_control.dram_wr_ack(dram_wr_ack);
  u_control.fpu_dout(fpu_dout);
  u_control.fpu_valid(fpu_valid);
  u_control.mmx_dout(mmx_dout);
  u_control.mmx_valid(mmx_valid);
  u_control.clear_branch(clear_branch);
  u_control.display_done(display_done);
  u_control.pc(pc);
  u_control.rfu_data1(rfu_data1);
  u_control.rfu_data2(rfu_data2);

  u_control.next_pc(next_pc);
  u_control.branch_valid(branch_valid);
  u_control.branch_target_address(branch_target_address);
  u_control.dram_cs(dram_cs);
  u_control.dram_addr(dram_addr);
  u_control.dram_datain(dram_datain);
  u_control.alu_op(alu_op);
  u_control.dram_we(dram_we);
  u_control.src_A(src_A);
  u_control.src_B(src_B);
  u_control.alu_cs(alu_cs);
  u_control.fpu_cs(fpu_cs);
  u_control.mmx_cs(mmx_cs);
  u_control.pid_valid(pid_valid);
  u_control.pid_data(pid_data);
  u_control.rfu_rst(rfu_rst);
  u_control.rfu_cs(rfu_cs);
  u_control.rfu_re(rfu_re);
  u_control.rfu_we(rfu_we);
  u_control.rfu_addr1(rfu_addr1);
  u_control.rfu_addr2(rfu_addr2);
  u_control.rfu_addr3(rfu_addr3);
  u_control.rfu_data3(rfu_data3);

  // connect to test bench
  u_test.clk(clk);
  u_test.next_pc(next_pc);
  u_test.instruction_out(instruction);
  u_test.instruction_valid_out(instruction_valid);
  u_test.alu_valid_out(alu_valid);
  u_test.alu_dout(alu_dout);
  u_test.dram_dout(dram_dout);
  u_test.dram_rd_valid_out(dram_rd_valid);
  u_test.dram_wr_ack(dram_wr_ack);
  u_test.fpu_dout(fpu_dout);
  u_test.fpu_valid_out(fpu_valid);
  u_test.mmx_dout(mmx_dout);
  u_test.mmx_valid_out(mmx_valid);
  u_test.clear_branch(clear_branch);
  u_test.display_done(display_done);
  u_test.pc_out(pc);
  u_test.rfu_data1_out(rfu_data1);
  u_test.rfu_data2_out(rfu_data2);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("risc_control");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, resetin, "resetin");
  sc_trace(trace_file, instruction, "instruction");
  sc_trace(trace_file, instruction_valid, "instruction_valid");
  sc_trace(trace_file, alu_valid, "alu_valid");
  sc_trace(trace_file, alu_dout, "alu_dout");
  sc_trace(trace_file, dram_dout, "dram_dout");
  sc_trace(trace_file, dram_rd_valid, "dram_rd_valid");
  sc_trace(trace_file, dram_wr_ack, "dram_wr_ack");
  sc_trace(trace_file, fpu_dout, "fpu_dout");
  sc_trace(trace_file, fpu_valid, "fpu_valid");
  sc_trace(trace_file, mmx_valid, "mmx_valid");
  sc_trace(trace_file, clear_branch, "clear_branch");
  sc_trace(trace_file, display_done, "display_done");
  sc_trace(trace_file, pc, "pc");
  sc_trace(trace_file, rfu_data1, "rfu_data1");
  sc_trace(trace_file, rfu_data2, "rfu_data2");
  sc_trace(trace_file, next_pc, "next_pc");
  sc_trace(trace_file, branch_valid, "branch_valid");
  sc_trace(trace_file, branch_target_address, "branch_target_address");
  sc_trace(trace_file, dram_cs, "dram_cs");
  sc_trace(trace_file, dram_addr, "dram_addr");
  sc_trace(trace_file, dram_datain, "dram_datain");
  sc_trace(trace_file, alu_op, "alu_op");
  sc_trace(trace_file, dram_we, "dram_we");
  sc_trace(trace_file, src_A, "src_A");
  sc_trace(trace_file, src_B, "src_B");
  sc_trace(trace_file, alu_cs, "alu_cs");
  sc_trace(trace_file, fpu_cs, "fpu_cs");
  sc_trace(trace_file, mmx_cs, "mmx_cs");
  sc_trace(trace_file, pid_valid, "pid_valid");
  sc_trace(trace_file, pid_data, "pid_data");
  sc_trace(trace_file, rfu_rst, "rfu_rst");
  sc_trace(trace_file, rfu_cs, "rfu_cs");
  sc_trace(trace_file, rfu_re, "rfu_re");
  sc_trace(trace_file, rfu_we, "rfu_we");
  sc_trace(trace_file, rfu_addr1, "rfu_addr1");
  sc_trace(trace_file, rfu_addr2, "rfu_addr2");
  sc_trace(trace_file, rfu_addr3, "rfu_addr3");
  sc_trace(trace_file, rfu_data3, "rfu_data3");
#endif

  sc_start(500, SC_NS );

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
