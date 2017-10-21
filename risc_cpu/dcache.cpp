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

  dcache.cpp -- Data Cache Unit.

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


#include "systemc.h"
#include "dcache.h"

// LCOV_EXCL_BR_START
sc_int<32> dcache::dmemory[DCACHE_SIZE];	 			// data memory
// LCOV_EXCL_BR_STOP

void dcache::entry()
{
  sc_uint<32>	address; // LCOV_EXCL_BR_LINE
  bool cs_tmp = false;
  bool we_tmp = false;

  while (true) {
    wait();// LCOV_EXCL_BR_LINE
    cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
    while(cs_tmp == false){
      wait();// LCOV_EXCL_BR_LINE
      cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
    }

   	address = addr.read();// LCOV_EXCL_BR_LINE

    we_tmp = we.read();// LCOV_EXCL_BR_LINE
   	if (we_tmp == true){
      // Write operation
      // LCOV_EXCL_BR_START
      wait(wait_cycles);
      dmemory[address] = datain.read();
      wr_ack.write(true);
      wait();
      wr_ack.write(false);
      // LCOV_EXCL_BR_STOP
    } else {
      // Read operation
      // LCOV_EXCL_BR_START
        wait(wait_cycles);
        dout.write(dmemory[address]);
        out_valid.write(true);
        wait();
        out_valid.write(false);
     // LCOV_EXCL_BR_STOP
    }
  }//LCOV_EXCL_LINE
} // LCOV_EXCL_BR_LINE

