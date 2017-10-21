//===========================================================================
// File Name    : main.cpp
// Description  : SNOW 3G  testbench
// Release Date : 24/07/2013
//
// Revision History
//---------------------------------------------------------------------------
// Date             Author      Version     Description
//---------------------------------------------------------------------------
//14/02/2013        PolyU            01
//
//// g++ -o snow_3G.exe main.cpp tb_snow_3G.cpp fir.cpp -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib -lsystemc -lm
//===========================================================================

#include "snow_3G.h"
#include "tb_snow_3G.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char** argv)
{
  sc_clock clk("clk", 10, SC_NS);
  sc_signal<bool>         rst;
  sc_signal<sc_uint<32> >  in_data[SIZE];
  sc_signal<sc_uint<32> >  out_data[SIZE];

  int x;
  char name[10];

  snow_3G u_SNOW("SNOW");
  test_snow_3G test("test_SNOW");

  //connect to SNOW 3G
  u_SNOW.clk( clk );
  u_SNOW.rst( rst );

  for(x=0; x < SIZE; x++){
    u_SNOW.ks_in[x]( in_data[x] );
    u_SNOW.ks_out[x]( out_data[x] );
  }

  // connect to test bench
  test.clk( clk );
  test.rst( rst );

  for(x=0; x < SIZE; x++){
    test.ks_in[x]( in_data[x] );
    test.ks_out[x]( out_data[x] );
  }

#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");
  // Top level signals
  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, rst, "rst");

  for(x=0; x < SIZE; x++){
    sprintf(name, "ks_in%d",x);
    sc_trace(trace_file, in_data[x], name);

    sprintf(name, "ks_out%d",x);
    sc_trace(trace_file, out_data[x], name);
  }
#endif

  sc_start( 10, SC_NS );
  rst.write(0);

  sc_start( 10, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

    return 0;
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
