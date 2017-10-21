//========================================================================================
//
// File Name    : filter_interp.h
// Description  : Main interpolation filter component declaration
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date       Version   Author      Description
//----------------------------------------------------------------------------------------
//23/07/2013   1.0      PolyU       Interpolation filter declaration file
//=======================================================================================

#ifndef INTERP_H_
#define INTERP_H_

#define SC_INCLUDE_FX

#include "define.h"

//LCOV_EXCL_BR_START
SC_MODULE (interp) {
public:
  sc_in_clk clk;
  sc_in<bool> rst;
  sc_in< sc_fixed<16,1,SC_TRN,SC_WRAP> > indata;
  sc_in< sc_ufixed<9,0,SC_TRN,SC_WRAP> > infactor;

  sc_out< bool > odata_en;
  sc_out< sc_fixed<16,1,SC_RND,SC_SAT> > odata;

  sc_fixed<16,1,SC_TRN,SC_WRAP> buffer[TAPS];
  sc_fixed<16,1,SC_TRN,SC_WRAP> indata_read;
  sc_ufixed<9,0,SC_TRN,SC_WRAP> infactor_read;
  sc_fixed<16,1,SC_RND,SC_SAT> odata_write;

  sc_fixed<36,6,SC_TRN,SC_WRAP> SoP1;
  sc_fixed<36,6,SC_TRN,SC_WRAP> SoP2;
  sc_fixed<36,6,SC_TRN,SC_WRAP> SoP3;
  sc_fixed<36,6,SC_TRN,SC_WRAP> SoP4;

  /* R */
  void run (void);

  SC_CTOR (interp) {
    SC_CTHREAD (run, clk.pos());
    reset_signal_is (rst, false );
  }

  ~interp() {}
};
//LCOV_EXCL_BR_STOP

#endif //  INTERP_H_
