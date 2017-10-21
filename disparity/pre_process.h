//=================================================================================
//
// File Name    : pre_process.h
// Description  : RGB to YUV conversion and parameter computation
// Release Date : 24/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date       Version    Author     Description
//---------------------------------------------------------------------------
//24/07/2013     1.0                 RGB to YUV conversion and parameter compuation
//================================================================================

#ifndef PRE_PROCESS_
#define PRE_PROCESS_

#include <systemc.h>

//LCOV_EXCL_BR_START
SC_MODULE (pre_process) {

  // inputs
  sc_in<bool>          clk;
  sc_in<bool>          rst;

  sc_in<bool>          VSynch;
  sc_in<bool>          HSynch;
  sc_in<sc_uint<16> >  Height;
  sc_in<sc_uint<16> >  Width;
  sc_in<sc_uint<8> >   Left_pixel_R;
  sc_in<sc_uint<8> >   Left_pixel_G;
  sc_in<sc_uint<8> >   Left_pixel_B;
  sc_in<sc_uint<8> >   Right_pixel_R;
  sc_in<sc_uint<8> >   Right_pixel_G;
  sc_in<sc_uint<8> >   Right_pixel_B;
  sc_in<bool>          valid_in;


  // outputs
  sc_out<bool>          VSynch_load;
  sc_out<bool>          HSynch_load;
  sc_out<sc_uint<16> >  Height_load;
  sc_out<sc_uint<16> >  Width_load;
  sc_out<sc_uint<8> >   Left_pixel_Y_load;
  sc_out<sc_uint<8> >   Right_pixel_Y_load;
  sc_out<bool>          valid_in_load;

  sc_out<sc_uint<10> > hsize;
  sc_out<sc_uint<10> > wsize;
  sc_out<sc_uint<7> >  hd;
  sc_out<sc_uint<7> >  wd;
  sc_out<sc_uint<10> > hbnd;
  sc_out<sc_uint<10> > wbnd;


  // Variables declaration
  sc_signal<bool>      VSynch_read;


  // Member functions declaration

  /* P */
  void parameters();

  /* R */
  void RGBtoYUV();



  SC_CTOR (pre_process){

    SC_CTHREAD(RGBtoYUV,clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(parameters,clk.pos());
    reset_signal_is(rst,false);

  };

  ~pre_process(){};

};
//LCOV_EXCL_BR_STOP
#endif // PRE_PROCESS_
