
//========================================================================================
//
// File Name    : tb_interp.h
// Description  : Header for the testbench
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date           Version   Author     Description
//----------------------------------------------------------------------------------------
// 23/07/2013        1.0    PolyU      TB header file
//=======================================================================================
#ifndef TB_INTERP_
#define TB_INTERP_


#define SC_INCLUDE_FX

#include "define.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_interp){
public:
  sc_in_clk clk;
  sc_in<bool> rst;
  sc_in<bool> odata_en;
  sc_in<sc_fixed <16,1,SC_RND,SC_SAT> > odata;

  sc_out<sc_fixed <16,1,SC_TRN,SC_WRAP> > indata;
  sc_out<sc_ufixed<9,0,SC_TRN,SC_WRAP> > infactor;

  FILE *fptr_data;
  FILE *fptr_en;
  FILE *in_file_data, *in_file_factor;

  /* C */
  void compare_results();
  /* R */
  void monitor();
  /* S */
  void driver();

  SC_CTOR(tb_interp){
    SC_CTHREAD(driver, clk.pos());
    reset_signal_is(rst, false);

    SC_CTHREAD(monitor, clk.pos());
    reset_signal_is(rst, false);
  }

 ~tb_interp(){}
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif  // TB_INTERP
