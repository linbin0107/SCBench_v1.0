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

#include "systemc.h"
#include "registerfile.h"

void registerfile::read1()
{
  bool rst_tmp;
  bool cs_tmp;
  bool re_tmp;

  wait();//LCOV_EXCL_BR_LINE
  while(true) {
    // LCOV_EXCL_BR_START
    rst_tmp = rst.read();
    cs_tmp = cs.read();
    re_tmp = re.read();
    // LCOV_EXCL_BR_STOP
    if(rst_tmp == true)
    {
      data1.write(0); // LCOV_EXCL_BR_LINE
    }
    else if((cs_tmp == true) && (re_tmp == true))
    {
      data1.write(reg[addr1.read()]);// LCOV_EXCL_BR_LINE
    }

    wait();// LCOV_EXCL_BR_LINE
  }
}

void registerfile::read2()
{
  bool rst_tmp;
  bool cs_tmp;
  bool re_tmp;

  wait();//LCOV_EXCL_BR_LINE
  while(true){
    // LCOV_EXCL_BR_START
    rst_tmp = rst.read();
    cs_tmp = cs.read();
    re_tmp = re.read();
    // LCOV_EXCL_BR_STOP
    if(rst_tmp == true)
    {
      data2.write(0); // LCOV_EXCL_BR_LINE
    }
    else if((cs_tmp == true) && (re_tmp == true))
    {
      data2.write(reg[addr2.read()]);//LCOV_EXCL_BR_LINE
    }

    wait();//LCOV_EXCL_BR_LINE
  }
}

void registerfile::write3()
{
  bool rst_tmp;
  bool cs_tmp;
  bool we_tmp;

  wait();//LCOV_EXCL_BR_LINE
  while(true){
    // LCOV_EXCL_BR_START
    rst_tmp = rst.read();
    cs_tmp = cs.read();
    we_tmp = we.read();
    // LCOV_EXCL_BR_STOP
	if(rst_tmp == true)
    {}
    else if((cs_tmp == true) && (we_tmp == true))
    {
      reg[addr3.read()] = data3.read();//LCOV_EXCL_BR_LINE
    }

    wait();//LCOV_EXCL_BR_LINE
  }
}

void registerfile::flush()
{
  bool rst_tmp;

  wait();//LCOV_EXCL_BR_LINE
  while(true) {
    rst_tmp = rst.read(); // LCOV_EXCL_BR_LINE
    if(rst_tmp == true)
    {
      for(unsigned i = 0; i < 16; i++)
      {
        reg[i] = 0;
      }
    }

    wait(); // LCOV_EXCL_BR_LINE
  }
}// LCOV_EXCL_BR_LINE
