/***************************************************
 * File: tb_control.h
 * Description: testbench file for control unit
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 04/01/2015
 **************************************************/

#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_control){
  sc_in<bool> clk;
  sc_in<bool> next_pc;

  sc_out<sc_uint<32> > instruction_out;
  sc_out< bool > instruction_valid_out;
  sc_out<bool > alu_valid_out;
  sc_out<sc_int<32> > alu_dout;
  sc_out<sc_int<32> > dram_dout;
  sc_out<bool > dram_rd_valid_out;
  sc_out<bool > dram_wr_ack;
  sc_out<sc_int<32> > fpu_dout;
  sc_out<bool > fpu_valid_out;
  sc_out<sc_int<32> > mmx_dout;
  sc_out<bool > mmx_valid_out;
  sc_out<bool > clear_branch;
  sc_out<bool > display_done;
  sc_out<sc_uint<32> > pc_out;
  sc_out<sc_int<32> > rfu_data1_out;
  sc_out<sc_int<32> > rfu_data2_out;

  void driver();

  SC_CTOR(tb_control) {
    SC_CTHREAD(driver, clk.pos());
  }

  ~tb_control() { }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
