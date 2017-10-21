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

  paging.cpp -- Instruction Paging Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: various code updates

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "paging.h"

#include <iostream>
using namespace std;

void paging::entry()
{
  int address=0;
  bool csin_tmp = false;
  bool icache_validin_tmp = false;
  bool bios_validin_tmp = false;

  while (true)
  {
    wait();// LCOV_EXCL_BR_LINE
    csin_tmp = csin.read();// LCOV_EXCL_BR_LINE
    while (csin_tmp != true) {
      wait();// LCOV_EXCL_BR_LINE
      csin_tmp = csin.read();// LCOV_EXCL_BR_LINE
    }

    address = logical_address.read();// LCOV_EXCL_BR_LINE

    if (address >= 5)  // use ICACHE
    {
      // LCOV_EXCL_BR_START
      physical_address.write(logical_address.read());
      icache_csout.write(true);
      wait();
      wait();
      icache_validin_tmp = icache_validin.read();
      // LCOV_EXCL_BR_STOP
      while (icache_validin_tmp != true) {
        wait();// LCOV_EXCL_BR_LINE
        icache_validin_tmp = icache_validin.read();// LCOV_EXCL_BR_LINE
      }
      // LCOV_EXCL_BR_START
      dout.write(icache_din.read());
      data_valid.write(true);
      icache_csout.write(false);
      wait();
      data_valid.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    } else { // use BIOS
      // LCOV_EXCL_BR_START
      physical_address.write(logical_address.read());
      bios_csout.write(true);
      wait();
      wait();
      bios_validin_tmp = bios_validin.read();
      // LCOV_EXCL_BR_STOP
      while (bios_validin_tmp != true) {
        wait();// LCOV_EXCL_BR_LINE
        bios_validin_tmp = bios_validin.read();// LCOV_EXCL_BR_LINE
      }
      // LCOV_EXCL_BR_START
      dout.write(bios_din.read());
      data_valid.write(true);
      bios_csout.write(false);
      wait();
      data_valid.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    }
  }
} // LCOV_EXCL_BR_LINE

