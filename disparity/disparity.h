//==========================================================================-
//
// File Name    : disparity.h
// Description  : Disparity estimator module declaration
// Release Date : 16/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date     Version    Author       Description
//---------------------------------------------------------------------------
//16/07/13      1.0                 Main image disparity computation
//===========================================================================
#ifndef DISPARITY_H
#define DISPARITY_H
#include "define.h"

//LCOV_EXCL_BR_START
SC_MODULE (disparity) {
  // Inputs
  sc_in<bool>         clk;
  sc_in<bool>         rst;

  sc_in<bool>         VSynch;
  sc_in<bool>         HSynch;
  sc_in<sc_uint<16> > Height;
  sc_in<sc_uint<16> > Width;
  sc_in<sc_uint<8> >  Left_pixel_Y;
  sc_in<sc_uint<8> >  Right_pixel_Y;
  sc_in<bool>         valid_in;

  sc_in<sc_uint<10> > hsize;
  sc_in<sc_uint<10> > wsize;
  sc_in<sc_uint<7> >  hd;
  sc_in<sc_uint<7> >  wd;
  sc_in<sc_uint<10> > hbnd;
  sc_in<sc_uint<10> > wbnd;


  // Outputs
  sc_out<bool>        valid_out;
  sc_out<sc_uint<8> > depth_out;


  // Internal Variables
  sc_uint<8>           Right_buffer[FULL];
  sc_uint<8>           Left_buffer [MAX_DIFF];
  sc_int<20>           sad[MAX_DIFF][FULL];  // sum of absolute differences

  sc_uint<20>         tmp_sum[MAX_DIFF];
  sc_uint<20>         final_sum[MAX_DIFF];

   sc_uint<8> pixel_Ly,pixel_Ry;
   sc_int<9>  pixel_LR_diff;
   sc_int<20> pixel_LR_diff_adj;

   // counters declarations -Declared as integers. HLS tool should optimize their bw
  int count_skip_line;
  int count_skip_pixel ;
  int count_hd;
  int count_wd;
  int count_delay;
  int count_hsize;
  int count_wsize;
  int count_pixel;
  int count_line;

  sc_signal<sc_uint<1> > HSynch_pulse;
  sc_signal<sc_uint<1> > HSynch_d1;
  sc_int<20>  sad_single[MAX_DIFF];

  int         level, tlevel;
  sc_int<20>  Lsad;

  int         minD;
  sc_uint<8>  out;


  /* H */
  void HSynch_control();

  /* M */
  int MAX(int x, int y);
  int MIN(int x, int y);

  /* R */
  void run();
  sc_int<20> Read_SAD(sc_uint<7> idx, sc_uint<8> addr);

  /* W */
  void Write_SAD(sc_uint<7> idx, sc_uint<8> addr, sc_int<20> data);



  SC_CTOR (disparity){

    SC_CTHREAD(run,clk.pos());
    reset_signal_is(rst,false);

    SC_CTHREAD(HSynch_control,clk.pos());
    reset_signal_is(rst,false);
    }

  ~disparity(){}

};
//LCOV_EXCL_BR_STOP
#endif  // DISPARITY_H
