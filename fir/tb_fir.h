//========================================================================================
//
// File Name    : tb_fir.h
// Description  : Testbench
// Release Date : 14/02/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version    Author      Description
//---------------------------------------------------------------------------------------
//14/02/2013      1.0       PolyU     FIR filter original testbench
//=======================================================================================


#include "define.h"


#ifndef TB_FIR_H_
#define TB_FIR_H_

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_FIR){
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<sc_uint<8> > filter_output;

  sc_out<sc_uint<8> >  in_data[FILTER_TAPS];
  sc_out<sc_uint<16> > coeff[FILTER_TAPS];

  FILE * in_filter_file, *in_coeff_file, *out_filter_golden_file, *out_filter_file_read;
  FILE  *out_filter_file, *diff_file;

  /* C */
  void compare_results();

  /* R  */
  void driver();

  /* S */
  void monitor();

  SC_CTOR ( test_FIR ) {
    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

  ~test_FIR(){}

};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif
