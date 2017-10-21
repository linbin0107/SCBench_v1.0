//===========================================================================
//
//
// File Name    : main.cpp
// Description  : Real 3D disparity estimation
// Release Date : 16/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date         Version   Author    Description
//---------------------------------------------------------------------------
//16/07/13         1.0              top level main
///===========================================================================

#include "disparity_top.h"
#include "tb_disparity.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char** argv)
{
  sc_clock clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
  sc_signal<bool> rst;
  sc_signal<bool> VSynch;
  sc_signal<bool> HSynch;
  sc_signal<sc_uint<16> > Height;
  sc_signal<sc_uint<16> > Width;
  sc_signal<sc_uint<8> >  Left_pixel_R;
  sc_signal<sc_uint<8> >  Left_pixel_G;
  sc_signal<sc_uint<8> >  Left_pixel_B;
  sc_signal<sc_uint<8> >  Right_pixel_R;
  sc_signal<sc_uint<8> >  Right_pixel_G;
  sc_signal<sc_uint<8> >  Right_pixel_B;
  sc_signal<bool> valid_in;
  sc_signal<bool> valid_out;

  sc_signal<sc_uint<8> >  depth_out;
  disparity_top u_DISPARITY_TOP("DISPARITY");
  test_disparity u_test("test_DE");

  // Instantiate Disparity Estimator unit
  u_DISPARITY_TOP.clk( clk );
  u_DISPARITY_TOP.rst( rst );
  u_DISPARITY_TOP.VSynch( VSynch );
  u_DISPARITY_TOP.HSynch( HSynch );
  u_DISPARITY_TOP.Height( Height );
  u_DISPARITY_TOP.Width( Width );
  u_DISPARITY_TOP.Left_pixel_R( Left_pixel_B );
  u_DISPARITY_TOP.Left_pixel_G( Left_pixel_G );
  u_DISPARITY_TOP.Left_pixel_B( Left_pixel_R );
  u_DISPARITY_TOP.Right_pixel_R( Right_pixel_B );
  u_DISPARITY_TOP.Right_pixel_G( Right_pixel_G );
  u_DISPARITY_TOP.Right_pixel_B( Right_pixel_R );
  u_DISPARITY_TOP.valid_in( valid_in );
  u_DISPARITY_TOP.valid_out( valid_out );
  u_DISPARITY_TOP.depth_out( depth_out );

  // Instantiate TB
  u_test.clk( clk );
  u_test.rst( rst );
  u_test.VSynch( VSynch );
  u_test.HSynch( HSynch );
  u_test.Height( Height );
  u_test.Width( Width );
  u_test.Left_pixel_R( Left_pixel_B );
  u_test.Left_pixel_G( Left_pixel_G );
  u_test.Left_pixel_B( Left_pixel_R );
  u_test.Right_pixel_R( Right_pixel_B );
  u_test.Right_pixel_G( Right_pixel_G );
  u_test.Right_pixel_B( Right_pixel_R );
  u_test.valid_in( valid_in );
  u_test.valid_out( valid_out );
  u_test.depth_out( depth_out );


  // Trace files
#ifdef WAVE_DUMP
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signal
  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, rst, "rst");
  sc_trace(trace_file, Left_pixel_R, "Left_pixel_R");
  sc_trace(trace_file, Left_pixel_G, "Left_pixel_G");
  sc_trace(trace_file, Left_pixel_B, "Left_pixel_B");
  sc_trace(trace_file, Right_pixel_R, "Right_pixel_R");
  sc_trace(trace_file, Right_pixel_G, "Right_pixel_G");
  sc_trace(trace_file, Right_pixel_B, "Right_pixel_B");
  sc_trace(trace_file, VSynch, "VSynch");
  sc_trace(trace_file, HSynch, "HSynch");
  sc_trace(trace_file, valid_in, "valid_in");
  sc_trace(trace_file, valid_out, "valid_out");
  sc_trace(trace_file, depth_out, "depth_out");
  sc_trace(trace_file, u_test.frame, "frame");
  sc_trace(trace_file, u_test.y, "y");
  sc_trace(trace_file, u_test.x, "x");

  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->Height, "u_DISPARITY_TOP.u_DISPARITY.>Height");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->Width, "u_DISPARITY_TOP.u_DISPARITY.Width");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->Left_pixel_Y, "u_DISPARITY_TOP.u_DISPARITY.Left_pixel_Y");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->Right_pixel_Y, "u_DISPARITY_TOP.u_DISPARITY_.Right_pixel_Y");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->valid_in, "u_DISPARITY_TOP.u_DISPARITY.valid_in");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->hsize, "u_DISPARITY_TOP.u_DISPARITY.hsize");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->wsize, "u_DISPARITY_TOP.u_DISPARITY.wsize");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->hd, "u_DISPARITY_TOP.u_DISPARITY.hd");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->wd, "u_DISPARITY_TOP.u_DISPARITY.wd");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->hbnd, "u_DISPARITY_TOP.u_DISPARITY.hbnd");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->wbnd, "u_DISPARITY_TOP.u_DISPARITY.wbnd");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->valid_out, "u_DISPARITY_TOP.u_DISPARITY.valid_out");
  sc_trace(trace_file, u_DISPARITY_TOP.u_DISPARITY->depth_out, "u_DISPARITY_TOP.u_DISPARITY.depth_out");
#endif  // End WAVE_DUMP


  sc_start( 25, SC_NS );
  rst.write(0);

  sc_start( 25, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
