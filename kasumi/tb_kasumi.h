//========================================================================================
//
//
// File Name    : tb_kasumi.h
// Description  : Testbench
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version  Author        Description
//---------------------------------------------------------------------------------------
//23/07/2013     1.0    PolyU        Kasumi  testbench
//=======================================================================================


#ifndef TB_KASUMI_H
#define TB_KASUMI_H

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_kasumi){
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<sc_uint<8> >  odata[DEPTH][WIDTH];

  sc_out<sc_uint<8> >  indata[DEPTH][WIDTH];
  sc_out<sc_uint<8> >  k[8][2];

  FILE *in_file, *in_kfile,  *out_golden_file, *out_kasumi_file;
  FILE *out_file, *diff_file;

  void driver();
  void monitor();
  void compare_results();

  SC_CTOR ( test_kasumi ) {
    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

  ~test_kasumi(){}
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif
