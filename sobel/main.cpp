//========================================================================================
// File Name    : main.cpp
// Description  : Sobel filter top design implementation
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version    Author            Description
//---------------------------------------------------------------------------------------
//23/07/2013      1.0     PolyU DARC Lab     Top system
//
//// g++ -o sobel.exe main.cpp tb_sobel.cpp sobel.cpp -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib -lsystemc -lm
//=======================================================================================

#include "sobel.h"
#include "tb_sobel.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char* argv[]){
  sc_signal<bool> rst;

  sc_signal<sc_uint<8> > input_row[3];
  sc_signal<sc_uint<8> > output_row;
  int i;
  char name[15];

  sc_clock clk("clk", 10, SC_NS);

  sobel s_obj("sobel");
  tb_sobel tb_obj("tb_sobel");
  s_obj.clk(clk);
  s_obj.rst(rst);

  for(i=0;i< 3; i++){
    s_obj.input_row[i](input_row[i]);
  }

  s_obj.output_row(output_row);

  tb_obj.clk(clk);
  tb_obj.rst(rst);

  for(i=0;i< 3; i++){
    tb_obj.input_row[i](input_row[i]);
  }

  tb_obj.output_row(output_row);

#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");
  // Top level signals
  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, rst, "rst");

  for(i=0; i < SIZE_BUFFER; i++){
    sprintf(name, "input_row%d",i);
    sc_trace(trace_file, input_row[i], name);
  }

  sc_trace(trace_file, output_row, "output_row");
#endif  // End WAVE_DUMP

   sc_start(10, SC_NS );
   rst.write(0);

   sc_start(10, SC_NS );
   rst.write(1);

   sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
