//========================================================================================
//
// File Name    : fir.h
// Description  : FIR filter module declaration
// Release Date : 14/02/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version    Author      Description
//---------------------------------------------------------------------------------------
//14/02/2013      1.0       PolyU     FIR filter module declaration
//=======================================================================================

#ifndef FIR_H_
#define FIR_H_


#include "define.h"

//LCOV_EXCL_BR_START
SC_MODULE (fir) {
public:
  sc_in_clk clk;
  sc_in<bool> rst;
  sc_in<sc_uint<8> > in_data[FILTER_TAPS] ;
  sc_in<sc_uint<16> > coeff[FILTER_TAPS] ;

  sc_out< sc_uint<8> > filter_output ;

  sc_uint<8> filter(sc_uint<8> *, sc_uint<16> *);

  /* F */
  void fir_main ( void );

  // Constructor
  SC_CTOR (fir) {
    SC_CTHREAD (fir_main, clk.pos() );
    reset_signal_is(rst, false) ;
  }

  ~fir() {}
};
//LCOV_EXCL_BR_STOP

#endif

