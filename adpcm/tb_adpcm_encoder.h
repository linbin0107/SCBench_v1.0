//========================================================================================
//
// File Name    : tb_adpcm_encoder.h
// Description  : Testbench for adpcm encoder
// Release Date : 31/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date       Version   Author           Description
//---------------------------------------------------------------------------------------
//31/07/2013    1.0    PolyU DARC Lab   ADPCM encoder testbench module declaration
//=======================================================================================

#ifndef TEST_ADPCM
#define TEST_ADPCM

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_STOP
SC_MODULE (test_adpcm){
  // Inputs
  sc_in<bool> clk;
  sc_in<bool> rst;

  sc_in<sc_uint<4> > odata;

  //  Outputs
  sc_out<sc_uint<16> > idata;

  //For data feeding
  FILE * in_file, *out_file_golden, *out_file, *diff_file;

  /* C */
  void compare_results();

  void monitor();

  void driver();

  SC_CTOR ( test_adpcm ) {

    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst,false);
  };

  ~test_adpcm(){};

};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif // TEST_ADPCM
