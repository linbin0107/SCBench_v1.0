//========================================================================================
//
// File Name    : tb_qsort.h
// Description  : Testbench for Quick sort
// Release Date : 01/08/2013
// Author       : PolyU DARC Group
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Author     Version     Description
//---------------------------------------------------------------------------------------
// 01/08/2013  PolyU       1.0       Quick Sort original testbench
//=======================================================================================

#ifndef TEST_QSORT
#define TEST_QSORT

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_qsort){
  // Inputs
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<sc_uint<8> > odata;
  //  Outputs
  sc_out<sc_uint<8> > indata;

  //For data feeding
  FILE * in_file, *out_golden_file, *out_file_read;
  FILE  *out_file, *diff_file;

  /* C */
  void compare_results();
  /* R */
  void monitor();
  /* S */
  void driver();

  SC_CTOR ( test_qsort ) {
    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

  ~test_qsort(){}

};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif //TEST_QSORT
