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

  bios.cpp -- System Bios.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: major revision

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "bios.h"

// LCOV_EXCL_BR_START
sc_uint<32> bios::imemory[BOOT_LENGTH] =
{
0x0f000000,
0x0f000001,
0x0f000002,
0x0f000003,
0x0f000004,
0x0f000005
};
// LCOV_EXCL_BR_STOP

void bios::entry()
{
  bool cs_tmp = false;
  sc_uint<32> address; // LCOV_EXCL_BR_LINE

  while (true)
  {
    wait();// LCOV_EXCL_BR_LINE
    cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
    while(cs_tmp == false) {
      wait(); // LCOV_EXCL_BR_LINE
      cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
    }

    address = addr.read();//LCOV_EXCL_BR_LINE

    if (address < BOOT_LENGTH)
    {		// in BOOTING STAGE

     // Introduce delay needed
     // LCOV_EXCL_BR_START
     wait();
     wait();

     dataout.write(imemory[address]);
     bios_valid.write(true);
     wait();
     bios_valid.write(false);
     wait();
     // LCOV_EXCL_BR_STOP
  }
  else
  {
    bios_valid.write(false);// LCOV_EXCL_BR_LINE
    wait();// LCOV_EXCL_BR_LINE
  }
 }
} // LCOV_EXCL_BR_LINE

