//========================================================================================
//
// File Name    : tb_fft.h
// Description  : Testbench
// Release Date : 16/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version    Author            Description
//---------------------------------------------------------------------------------------
//16/07/2013      1.0    PolyU DARC Lab     FFT testbench top module declaration
//=======================================================================================

#ifndef TEST_FFT
#define TEST_FFT

#define SC_INCLUDE_FX
#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_fft){
  sc_in<bool> clk;
  sc_in<bool> rst;

  sc_in<sc_fixed<32,16, SC_TRN, SC_WRAP> > out_real;
  sc_in<sc_fixed<32,16, SC_TRN, SC_WRAP> > out_imag;
  sc_in<bool> data_req;
  sc_in<bool> data_ready;

  sc_out<sc_fixed<32,16, SC_TRN, SC_WRAP> > in_real;
  sc_out<sc_fixed<32,16, SC_TRN, SC_WRAP> > in_imag;
  sc_out<bool> data_valid;
  sc_out<bool> data_ack;

  FILE * in_file, *out_file_golden, *out_file, *diff_file;

  /* C */
  void compare_results();

  /* R */
  void driver();

  /* S */
  void monitor();

  SC_CTOR ( test_fft ) {
    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

  ~test_fft(){}
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif //TEST_FFT
