/***************************************************
 * File: tb_floating.h
 * Description: testbench header file for floating
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 09/01/2016
 **************************************************/

#ifndef _TB_FLOATING_H_
#define _TB_FLOATING_H_

#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_floating)
{
  sc_in_clk CLK;

  sc_in<sc_int<32> > result;// FPU output
  sc_in<bool> isValid;	// output valid

  sc_out<bool> cs;// chip select
  sc_out<sc_uint<8> > opcode;// opcode
  sc_out<sc_int<32> > dina;// operand A
  sc_out<sc_int<32> > dinb;// operand B

  void compare_results();

  void driver();
  void monitor();

  SC_CTOR(tb_floating)
  {
    SC_THREAD(driver);
    sensitive << CLK.pos();

    SC_THREAD(monitor);
    sensitive << CLK.pos();
  }

  ~tb_floating() { }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif
