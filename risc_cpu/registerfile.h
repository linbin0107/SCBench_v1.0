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

  registerfile_l.cpp -- Register File

  Original Author: Nicolas Blanc, ETHZ.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/
#ifndef _REGISTERFILE_H_
#define _REGISTERFILE_H_

#include "systemc.h"

// LCOV_EXCL_BR_START
SC_MODULE(registerfile)
{
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_in<bool> cs;
  sc_in<bool> re;
  sc_in<bool> we;
  sc_in<sc_uint<4> > addr1;
  sc_in<sc_uint<4> > addr2;
  sc_in<sc_uint<4> > addr3;

  sc_out<sc_int<32> > data1;
  sc_out<sc_int<32> > data2;

  sc_in<sc_int<32> > data3;

  sc_int<32> reg[16];

  SC_CTOR(registerfile)
  {
    SC_CTHREAD(read1, clk.pos());

    SC_CTHREAD(read2, clk.pos());

    SC_CTHREAD(write3, clk.pos());

    SC_CTHREAD(flush, clk.pos());
  }

  // The operations of the risc_registerfile:
  void read1();
  void read2();
  void write3();
  void flush();
};
// LCOV_EXCL_BR_STOP

#endif
