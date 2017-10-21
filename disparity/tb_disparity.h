//===========================================================================
//
// File Name    : test_disparity.h
// Description  : Testbench for disparity estimator
// Release Date : 16/07/13
//
//
// Revision History
//---------------------------------------------------------------------------
// Date           Version    Author    Description
//---------------------------------------------------------------------------
//16/07/13             1.0             Testbench send/receive and compare outputs
//===========================================================================

#ifndef TEST_DISPARITY
#define TEST_DISPARITY

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE (test_disparity) {

  // Inputs
    sc_in_clk clk;
  sc_in<bool>          rst;
  sc_in<bool>          valid_out;
  sc_in<sc_uint<8> >   depth_out;

  // Outputs
  sc_out<bool>         VSynch;
  sc_out<bool>         HSynch;
  sc_out<sc_uint<16> > Height;
  sc_out<sc_uint<16> > Width;

  sc_out<sc_uint<8> >  Left_pixel_R;
  sc_out<sc_uint<8> >  Left_pixel_G;
  sc_out<sc_uint<8> >  Left_pixel_B;
  sc_out<sc_uint<8> >  Right_pixel_R;
  sc_out<sc_uint<8> >  Right_pixel_G;
  sc_out<sc_uint<8> >  Right_pixel_B;
  sc_out<bool>         valid_in;


  //File pointers (bmp) input, outputs and difference
  FILE *in_file, *out_file, *compare_file, *diff_file;

  char BmpHeader1[18];
  char BmpHeader2[28];

  int frame, x, y;
  unsigned int height, width, plusbytes;

  // Pointers to bmp images loaded/generated
  unsigned char **inbmp;
  unsigned char **inbmpLeft;
  unsigned char**inbmpRight;
  unsigned char **outbmp;
  unsigned char **comparebmp;


  //For receiver
  bool          VSynch_single;
  bool          HSynch_single;
  sc_uint<16>   yi;
  sc_uint<7>    out_x, out_y;


  /* C */
  void compare_results();


  /* I */
  void initialize();

  /* R */
  void recv();

  /* S */
  void send();


  // Constructor declaration
  SC_CTOR (test_disparity){

      SC_CTHREAD(send,clk.pos());
       reset_signal_is(rst,false);

      SC_CTHREAD(recv,clk.pos());
      reset_signal_is(rst,false);
  };

  ~test_disparity(){};


};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif // TEST_DISPARITY
