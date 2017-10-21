//========================================================================================
//
// File Name    : tb_idct.h
// Description  : Testbench for IDCT
// Release Date : 31/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version  Author           Description
//---------------------------------------------------------------------------------------
//31/07/2013     1.0    PolyU DARC Lab    IDCT testbench
//=======================================================================================

#ifndef TEST_IDCT
#define TEST_IDCT

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_idct){
  // Inputs
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in< bool > output_start;
  sc_in< sc_uint<8> > output_sample;
  sc_in< sc_uint<3> > output_row;
  sc_in< sc_uint<3> > output_col;
  //  Outputs
  sc_out<  bool > input_ready;
  sc_out< sc_int<16> > input_coef;
  sc_out< sc_uint<6> > input_quant;

  //For data feeding
  FILE *in_file, *in_file_coef, *out_file_golden;
  FILE *out_file, *diff_file;
  /* C */
  void compare_results();
  /* R */
  void monitor();
  /* S */
  void driver();

  SC_CTOR ( test_idct ) {

    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

  ~test_idct(){}
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
#endif //TEST_IDCT
