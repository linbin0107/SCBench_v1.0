//========================================================================================
//
// File Name    : sobel.h
// Description  : SOBEL filter declaration
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version   Author              Description
//---------------------------------------------------------------------------------------
//23/07/2013       1.0     PolyU DARC Lab      main component declaration of SOBEL filter
//=======================================================================================

#ifndef SOBEL_H_
#define SOBEL_H_

#include "define.h"

//LCOV_EXCL_BR_START
SC_MODULE (sobel){
public:
  sc_in_clk clk;
  sc_in<bool> rst;
  sc_in <sc_uint <8> > input_row[SIZE_BUFFER];

  sc_out<sc_uint<8> > output_row;

  sc_uint<8> line_buffer[SIZE_BUFFER][SIZE_BUFFER];
  sc_uint<8> sobel_filter(sc_uint<8> *);

  /* S */
  void sobel_main(void);

  SC_CTOR (sobel){
    SC_CTHREAD(sobel_main,clk.pos());
    reset_signal_is(rst,false);
  }

  ~sobel(){};

};
//LCOV_EXCL_BR_STOP
#endif  // SOEBEL_H_
