/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  floating.cpp -- Floating Point Execution Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: many code updates

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "floating.h"

void floating::entry()
{
  //LCOV_EXCL_BR_START
  sc_int<32>  opcode_tmp = 0;
  sc_int<32>  dout_tmp = 0;
  sc_uint<32>  dest_tmp = 0;

  sc_uint<32>  fpua_sign_tmp;
  sc_uint<32>  fpua_exponent_tmp;
  sc_uint<32>  fpua_significand_tmp;
  sc_uint<32>  fpub_sign_tmp;
  sc_uint<32>  fpub_exponent_tmp;
  sc_uint<32>  fpub_significand_tmp;
  const char *opcode_encode="";
  sc_uint<32>  fpua_tmp;
  sc_uint<32>  fpub_tmp;

  sc_int<32>  exponent_diff_tmp = 0;
  sc_int<32>  exponent_tmp = 0;
  sc_uint<32>  significant_result = 0;
  sc_uint<32>  overflow_sign_tmp = 0;
  sc_uint<32>  result_exp_tmp = 0;
  sc_uint<32>  result_sign_tmp = 0;
  //LCOV_EXCL_BR_STOP

  // main loop
  // current implementation only worked if both operand have same sign bits
  // overflow is ignored.
  // initialization of output
  wait(3);//LCOV_EXCL_BR_LINE

  while(true)
  {
    do {
      wait();//LCOV_EXCL_BR_LINE
    } while ( !(cs == true) );

    //LCOV_EXCL_BR_START
    opcode_tmp = opcode.read();

    fpua_tmp = dina.read();
    fpub_tmp = dinb.read();
    //LCOV_EXCL_BR_STOP

    fpua_sign_tmp = (fpua_tmp & 0x80000000) >> 31 ;
    fpub_sign_tmp = (fpub_tmp & 0x80000000) >> 31 ;

    fpua_exponent_tmp = (fpua_tmp & 0x7f800000) >> 23 ;
    fpub_exponent_tmp = (fpub_tmp & 0x7f800000) >> 23 ;

    fpua_significand_tmp = (fpua_tmp & 0x007fffff) ;
    fpub_significand_tmp = (fpub_tmp & 0x007fffff) ;

    //LCOV_EXCL_BR_START
    exponent_diff_tmp = sc_int<32>(fpua_exponent_tmp) -
                        sc_int<32>(fpub_exponent_tmp);
    //LCOV_EXCL_BR_STOP

    if (exponent_diff_tmp > 0)
    {
      exponent_tmp = fpua_exponent_tmp;
      //printf("shift significant B to Right\n");
      fpub_significand_tmp = fpub_significand_tmp >>
                             exponent_diff_tmp ;
      fpub_exponent_tmp = fpua_exponent_tmp;
    }
    else
    {
      exponent_tmp = fpub_exponent_tmp;
      //printf("shift significant A to Right\n");
      fpua_significand_tmp = fpua_significand_tmp >>
                             exponent_diff_tmp ;
      fpua_exponent_tmp = fpub_exponent_tmp;
    }

    wait();//LCOV_EXCL_BR_LINE

    // output MUX
    switch (opcode_tmp)
    {
      case 0:         // Stall
        opcode_encode = "STALL";
        //LCOV_EXCL_BR_START
        cout << "opcode:  STALL" << endl;
        dout_tmp = dout_tmp;
        wait();
        //LCOV_EXCL_BR_STOP
        break;
      case 3:         // add
        opcode_encode = "FADD";
        //LCOV_EXCL_BR_START
        cout << "opcode:  FADD" << endl;
        significant_result = sc_int<32>(fpua_significand_tmp) +
                             sc_int<32>(fpub_significand_tmp);
        wait();
        //LCOV_EXCL_BR_STOP
        break;
      case 4:         // sub
        opcode_encode = "FSUB";
        //LCOV_EXCL_BR_START
        cout << "opcode:  FSUB" << endl;
        significant_result = sc_int<32>(fpua_significand_tmp) -
                             sc_int<32>(fpub_significand_tmp);
        wait();
        //LCOV_EXCL_BR_STOP
        break;
      case 5:         // mul
        opcode_encode = "FMUL";
        //LCOV_EXCL_BR_START
        cout << "opcode:  FMUL" << endl;
        significant_result = sc_int<32>(fpua_significand_tmp) *
                             sc_int<32>(fpub_significand_tmp);
        fpub_exponent_tmp *= 2; // exponent is doubled in value
        wait();
        //LCOV_EXCL_BR_STOP
        break;
      case 6:         // div
        opcode_encode = "FDIV";
        //LCOV_EXCL_BR_START
        cout << "opcode:  FDIV" << endl;
        significant_result = sc_int<32>(fpua_significand_tmp) /
                             sc_int<32>(fpub_significand_tmp);
        wait();
        //LCOV_EXCL_BR_STOP
        break;
      default:
        //LCOV_EXCL_BR_START
        cout << "FPU: Bad Opcode " << opcode_tmp << endl;
        wait();
        //LCOV_EXCL_BR_STOP
        break;
    }

    overflow_sign_tmp = (significant_result & 0xff800000) >> 23;
    dout_tmp = (significant_result << overflow_sign_tmp) & 0x007fffff;
    result_exp_tmp = fpub_exponent_tmp  + overflow_sign_tmp;
    dout_tmp = dout_tmp | ((result_exp_tmp << 23) & 0x7f800000);
    result_sign_tmp = fpua_sign_tmp;
    dout_tmp = dout_tmp | ((result_sign_tmp << 31) & 0x80000000);
    //LCOV_EXCL_BR_START
    dout.write(dout_tmp);

    out_valid.write(true);
    wait();
    out_valid.write(false);
    wait();
    //LCOV_EXCL_BR_STOP
  }
}//LCOV_EXCL_BR_LINE
