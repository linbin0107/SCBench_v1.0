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

  fetch.cpp -- Instruction Fetch Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "fetch.h"


void fetch::entry()
{
  // LCOV_EXCL_BR_START
  sc_uint<32> addr_tmp=0;
  sc_uint<32> datao_tmp=0;
  sc_uint<32> datai_tmp=0;
  bool ram_valid_tmp = false;
  bool next_pc_tmp = false;
  bool interrupt_tmp = false;
  bool branch_valid_tmp = false;

  addr_tmp = 0;
  datao_tmp = 0xdeadbeef;

  // Now booting from default values
  reset.write(true);
  ram_cs.write(true);
  address.write(addr_tmp);
  // LCOV_EXCL_BR_STOP

  wait();// LCOV_EXCL_BR_LINE
  ram_valid_tmp = ram_valid.read();// LCOV_EXCL_BR_LINE
  while(ram_valid_tmp == false) {
    wait();// LCOV_EXCL_BR_LINE
    ram_valid_tmp = ram_valid.read();// LCOV_EXCL_BR_LINE
  }

  // LCOV_EXCL_BR_START
  instruction_valid.write(true);
  instruction.write(datai_tmp);
  program_counter.write(addr_tmp);
  ram_cs.write(false);
  wait();
  instruction_valid.write(false);
  addr_tmp++;
  wait();
  // LCOV_EXCL_BR_STOP

  while (true)
  {
//    addr_tmp = addr_in.read();
    if (addr_tmp == 5)
    {
      reset.write(false);// LCOV_EXCL_BR_LINE
    }

    interrupt_tmp = interrupt.read();//LCOV_EXCL_BR_LINE
    if (interrupt_tmp == true)
    {
      // LCOV_EXCL_BR_START
      ram_cs.write(true);
      addr_tmp = int_vectno.read();
      wait(2);//memory_latency);
      datai_tmp = ramdata.read();

      instruction_valid.write(true);
      instruction.write(datai_tmp);
      ram_cs.write(false);
      interrupt_ack.write(true);
      // LCOV_EXCL_BR_STOP

      next_pc_tmp = next_pc.read(); // LCOV_EXCL_BR_LINE
      if (next_pc_tmp == true) {
         addr_tmp++;
      }

      // LCOV_EXCL_BR_START
      wait();
      instruction_valid.write(false);
      interrupt_ack.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    }

    branch_valid_tmp = branch_valid.read();// LCOV_EXCL_BR_LINE
    if(branch_valid_tmp == true)
    {
      addr_tmp = branch_address.read();// LCOV_EXCL_BR_LINE
    }
    else
    {
      addr_tmp += 1;
    }

    // LCOV_EXCL_BR_START
    ram_cs.write(true);
    address.write(addr_tmp);
    //std::cout << "FETCH address " << addr_tmp << "\n";
    wait();
    ram_valid_tmp = ram_valid.read();
    // LCOV_EXCL_BR_STOP
    while(ram_valid_tmp == false) {
      wait();// LCOV_EXCL_BR_LINE
      ram_valid_tmp = ram_valid.read();// LCOV_EXCL_BR_LINE
    }

    // LCOV_EXCL_BR_START
    datai_tmp = ramdata.read();

    instruction_valid.write(true);
    instruction.write(datai_tmp);
    program_counter.write(addr_tmp);
    ram_cs.write(false);
    wait();
    instruction_valid.write(false);
    wait();
    next_pc_tmp = next_pc.read();
    // LCOV_EXCL_BR_STOP
    while(next_pc_tmp == false) {
      wait();// LCOV_EXCL_BR_LINE
      next_pc_tmp = next_pc.read();// LCOV_EXCL_BR_LINE
    }
  }//LCOV_EXCL_LINE
} // LCOV_EXCL_BR_LINE

