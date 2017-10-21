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

  exec.cpp -- Integer Execution Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "risc_exec.h"

void risc_exec::entry()
{
  // LCOV_EXCL_BR_START
  sc_int<32> opcode_tmp = 0;
  sc_int<32> add1_tmp = 0;
  sc_int<32> dina_tmp = 0;
  sc_int<32> dinb_tmp = 0;
  sc_int<32> dout_tmp = 0;
  bool cs_tmp = false;

  isBusy.write(false);
  // LCOV_EXCL_BR_STOP

  // main loop
  // initialization of output
  wait(3); // LCOV_EXCL_BR_LINE
  while(1) {
    cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
    if (cs_tmp == true) {
      // LCOV_EXCL_BR_START
      isBusy.write(true);
      opcode_tmp = opcode.read();
      dina_tmp = dina.read();
      dinb_tmp = dinb.read();

      //cout << hex << "opcode: 0x" << opcode_tmp << "\n";
      //cout << "dina: 0x" << dina_tmp << "\n";
      //cout << "dinb: 0x" << dinb_tmp << "\n";
      // LCOV_EXCL_BR_STOP

      // output MUX
      switch (opcode_tmp) {
        case 0: // Stall
          dout_tmp = dout_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 1:// add with carry
          dout_tmp = dina_tmp + dinb_tmp + add1_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 2:// sub with carry
          dout_tmp = dina_tmp - dinb_tmp - add1_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 3:// add without carry
          dout_tmp = dina_tmp + dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 4:// sub without carry
          dout_tmp = dina_tmp - dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 5: // multiply assume 2 clock cycle multiplication
          dout_tmp = dina_tmp * dinb_tmp;
          wait(2); // LCOV_EXCL_BR_LINE
          break;
        case 6:// divide assume 2 clock cycle multiplication
          assert(dinb_tmp && "Division Exception - Divide by zero\n");
          dout_tmp = dina_tmp / dinb_tmp;
          wait(2); // LCOV_EXCL_BR_LINE
          break;
        case 7:         // bitwise NAND
          dout_tmp = ~(dina_tmp & dinb_tmp);
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 8:         // bitwise AND
          dout_tmp = dina_tmp & dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 9:         // bitwise OR
          dout_tmp = dina_tmp | dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 10:         // bitwise XOR
          dout_tmp = dina_tmp ^ dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 11:         // bitwise complement
          dout_tmp = ~ dina_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 12:         // left shift
          dout_tmp = dina_tmp << dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 13:         // right shift
          dout_tmp = dina_tmp >> dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        case 14: 	// modulo
          assert(dinb_tmp);
          dout_tmp = dina_tmp % dinb_tmp;
          wait(); // LCOV_EXCL_BR_LINE
          break;
        default:
          cout << "ALU: Bad Opcode (" << opcode_tmp << ")\n"; // LCOV_EXCL_BR_LINE
          break;
      }

      dout.write(dout_tmp); // LCOV_EXCL_BR_LINE
      out_valid.write(true); // LCOV_EXCL_BR_LINE

      if (dout_tmp == 0) {
        Z.write(true); // LCOV_EXCL_BR_LINE
      } else {
        Z.write(false); // LCOV_EXCL_BR_LINE
      }

      if (dout_tmp > (2^32)) {
        V.write(true); // LCOV_EXCL_BR_LINE
      } else {
        V.write(false); // LCOV_EXCL_BR_LINE
      }

      // LCOV_EXCL_BR_START
      wait();
      out_valid.write(false);
      isBusy.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    } else {
      wait(); // LCOV_EXCL_BR_LINE
    }
  }
} // LCOV_EXCL_BR_LINE
