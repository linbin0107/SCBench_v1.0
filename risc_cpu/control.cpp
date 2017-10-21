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

  control.cpp  -- Instruction Decode Unit.

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
#include "control.h"

void control::entry()
{
  // LCOV_EXCL_BR_START
  sc_uint<32> 	instr_tmp = 0;
  sc_uint<8> 	 opcode_tmp = 0;
  sc_uint<4>   regA_tmp = 0;
  sc_uint<4>   regB_tmp = 0;
  sc_uint<4>   regC_tmp = 0;
  sc_int<32>  imm_tmp = 0;
  sc_uint<12>  offset_tmp = 0;
  sc_int<32> label_tmp = 0;
  sc_uint<32> longlabel_tmp = 0;

  sc_int<32>  lastreg_tmp = 0;
  sc_int<32>	srcA_tmp = 0;
  sc_int<32>	srcB_tmp = 0;
  sc_int<32>	srcC_tmp = 0;

  bool instruction_valid_tmp = false;
  bool alu_valid_tmp = false;
  bool dram_rd_valid_tmp = false;
  bool fpu_valid_tmp = false;
  bool mmx_valid_tmp = false;

  branch_valid.write(false);
  alu_cs.write(false);
  fpu_cs.write(false);
  mmx_cs.write(false);

  wait(2);
  // LCOV_EXCL_BR_STOP

  while (true)
  {
    // LCOV_EXCL_BR_START
    next_pc.write(true);
    alu_cs.write(false);
    fpu_cs.write(false);
    mmx_cs.write(false);
    dram_cs.write(false);
    // LCOV_EXCL_BR_STOP
    wait();// LCOV_EXCL_BR_LINE
    instruction_valid_tmp = instruction_valid.read(); // LCOV_EXCL_BR_LINE
    while(instruction_valid_tmp == false){
      wait(); // LCOV_EXCL_BR_LINE
      instruction_valid_tmp = instruction_valid.read(); // LCOV_EXCL_BR_LINE
    }
    // LCOV_EXCL_BR_START
    next_pc.write(false);
    branch_valid.write(false);

    instr_tmp  = instruction.read();
    opcode_tmp = instr_tmp.range(31,24);
    regC_tmp   = instr_tmp.range(23,20);
    regA_tmp   = instr_tmp.range(19,16);
    regB_tmp   = instr_tmp.range(15,12);
    imm_tmp    = sc_int<32>(sc_uint<32>(instr_tmp.range(15,0)));
    offset_tmp = instr_tmp.range(11,0);
    label_tmp  = sc_int<16>(instr_tmp.range(15,0));
    longlabel_tmp = instr_tmp.range(23,0);

    srcA_tmp = 0xdeadbeef;
    srcB_tmp = 0xdeadbeef;
    srcC_tmp = 0xdeadbeef;

    std::cout << pc.read() << "\t| ";
    // LCOV_EXCL_BR_STOP
    // get values form RFU
    switch(opcode_tmp)
    {
      case 0x4e:	// sw R1, R2, offset
      case 0x10:	// beq R1, R2, label
      case 0x11:	// bne R1, R2, label
      case 0x12:	// bgt R1, R2, label
      case 0x13:	// bge R1, R2, label
      case 0x14:	// blt R1, R2, label
      case 0x15:	// ble R1, R2, label
        // LCOV_EXCL_BR_START
        rfu_cs.write(true);
        rfu_re.write(true);
        rfu_addr1.write(regC_tmp);
        rfu_addr2.write(regA_tmp);
        wait();
        rfu_cs.write(false);
        rfu_re.write(false);
        wait();
        srcC_tmp = rfu_data1.read();
        srcA_tmp = rfu_data2.read();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x18:	// jr
        // LCOV_EXCL_BR_START
        rfu_cs.write(true);
        rfu_re.write(true);
        rfu_addr1.write(15);
        wait();
        rfu_cs.write(false);
        rfu_re.write(false);
        wait();
        srcC_tmp = srcA_tmp = srcB_tmp = rfu_data1.read();
        // LCOV_EXCL_BR_STOP
        break;
      default:    // Other cases
        // LCOV_EXCL_BR_START
        rfu_cs.write(true);
        rfu_re.write(true);
        rfu_addr1.write(regA_tmp);
        rfu_addr2.write(regB_tmp);
        wait();
        rfu_cs.write(false);
        rfu_re.write(false);
        wait();
        srcA_tmp = rfu_data1.read();
        srcB_tmp = rfu_data2.read();
        // LCOV_EXCL_BR_STOP
    }

    // Execute operation
    switch(opcode_tmp) {
      case 0x0:	// halt
        cout << "halt\n"; // LCOV_EXCL_BR_LINE
        wait(); // LCOV_EXCL_BR_LINE
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x01:	// add R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "add R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp << "), R"
          << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(3);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait();  // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x02:	// addi R1, R2, #value
        // LCOV_EXCL_BR_START
        cout << "addi R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp  << "), " << imm_tmp <<  "\n";
        src_A.write(srcA_tmp);
        src_B.write(imm_tmp);
        alu_op.write(3);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x03:	// addc R1, R2, R3 + Carrybit
        // LCOV_EXCL_BR_START
        cout << "addc R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")+C\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(1);
        alu_cs.write(true);

        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x04:	// sub R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "sub R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(4);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x05:	// subi R1, R2,  #value
        // LCOV_EXCL_BR_START
        cout << "subi R" << regC_tmp << ", R" << regA_tmp
             << "(=" << srcA_tmp  << "), " << imm_tmp <<  "\n";
        src_A.write(srcA_tmp);
        src_B.write(imm_tmp);
        alu_op.write(4);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x06:	// subc R1, R2, R3 - Carrybit
        // LCOV_EXCL_BR_START
        cout << "subc R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")-C\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(2);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x07:	// mul R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "mul R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(5);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x08:	// div R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "div R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(6);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x09:	// nand R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "nand R" << regC_tmp << ", R" << regA_tmp << "(="  << srcA_tmp
          << "), R" << regB_tmp << "(="  << srcB_tmp  << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(8);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x0b:	// or R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "or R" << regC_tmp << ", R" << regA_tmp << "(="  << srcA_tmp
          << "), R" << regB_tmp << "(="  << srcB_tmp  << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(9);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x0c:	// xor R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "xor R" << regC_tmp << ", R" << regA_tmp << "(="  << srcA_tmp
          << "), R" << regB_tmp << "(="  << srcB_tmp  << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(10);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x0d:      // NOT R1, R2
        // LCOV_EXCL_BR_START
        cout << "not R" <<  regC_tmp << ", R" <<  regA_tmp << "(="  << srcA_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(0);
        alu_op.write(11);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }

        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x0e:      // modulo R1 = R2 mod R3
        // LCOV_EXCL_BR_START
        cout << "modulo R" << regC_tmp << ", R" << regA_tmp << "(="  << srcA_tmp
          << "), R" << regB_tmp << "(="  << srcB_tmp  << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(14);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read();//LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x4d:	// lw R1, R2, offset
        // LCOV_EXCL_BR_START
        cout << "lw R"<<  regC_tmp << ", R" <<  regA_tmp
             <<"(="  <<  srcA_tmp  << "), " <<  offset_tmp  << "\n";
        dram_cs.write(true);
        dram_we.write(false);
        offset_tmp = offset_tmp + srcA_tmp;
        dram_addr.write(offset_tmp);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        dram_rd_valid_tmp = dram_rd_valid.read(); // LCOV_EXCL_BR_LINE
        while(dram_rd_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          dram_rd_valid_tmp = dram_rd_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        dram_cs.write(false);
        srcC_tmp = dram_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x4e:	// sw R1, R2, offset
        // LCOV_EXCL_BR_START
        cout << "sw R"<<  regC_tmp << "(=" << srcC_tmp  <<"), R"
          << regA_tmp <<"(="  <<  srcA_tmp  << "), "
          <<  offset_tmp  << "\n";
        dram_cs.write(true);
        dram_we.write(true);
        offset_tmp = offset_tmp + srcA_tmp;
        dram_addr.write(offset_tmp);
        dram_datain.write(srcC_tmp);
        wait();
        dram_cs.write(false);
        dram_we.write(false);
        wait(3);
        // LCOV_EXCL_BR_STOP
        break;
      case 0x0f:	// mov R1, R2
        // LCOV_EXCL_BR_START
        cout << "mov R" <<  regC_tmp << ", R" <<  regA_tmp << "(=" <<  srcA_tmp <<")\n";
        src_A.write(srcA_tmp);
        src_B.write(0);
        alu_op.write(3);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x10:	// beq R1, R2, label
        cout << "beq R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" << srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp == srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x11:	// bne R1, R2, label
        cout << "bne R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" <<  srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp != srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x12:	// bgt R1, R2, label
        cout << "bgt R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" <<  srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp > srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x13:	// bge R1, R2, label
        cout << "bge R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" <<  srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp >= srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x14:	// blt R1, R2, label
        cout << "blt R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" <<  srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp < srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x15:	// ble R1, R2, label
        cout << "ble R" <<  regC_tmp << "(="<< srcC_tmp <<"), R" << regA_tmp << // LCOV_EXCL_BR_LINE
             "(=" <<  srcA_tmp << "), " << label_tmp  << "\n"; // LCOV_EXCL_BR_LINE
        if (srcC_tmp < srcA_tmp) {
          branch_target_address.write(pc.read() + sc_int<32>(label_tmp)); // LCOV_EXCL_BR_LINE
          branch_valid.write(true); // LCOV_EXCL_BR_LINE
        }
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x16:	// j label
        // LCOV_EXCL_BR_START
        cout <<"j "  <<  longlabel_tmp  << "\n";
        branch_target_address.write(longlabel_tmp);
        branch_valid.write(true);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x17:	// jal label for procedure call
        // LCOV_EXCL_BR_START
        cout <<"jal "  <<  longlabel_tmp  << "\n";
        branch_target_address.write(longlabel_tmp);
        branch_valid.write(true);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x18:	// jr
        // LCOV_EXCL_BR_START
        cout << "jr\n";
        branch_target_address.write(sc_uint<32>(srcC_tmp));
        branch_valid.write(true);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x29:      // fadd R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "fadd R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(3);
        fpu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        while(fpu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        fpu_cs.write(false);
        srcC_tmp = fpu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x2a:      // fsub R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "fsub R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(4);
        fpu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        while(fpu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        fpu_cs.write(false);
        srcC_tmp = fpu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x2b:      // fmul R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "fmul R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(5);
        fpu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        while(fpu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        fpu_cs.write(false);
        srcC_tmp = fpu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x2c:      // fdiv R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "fdiv R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(6);
        fpu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        while(fpu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          fpu_valid_tmp = fpu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        fpu_cs.write(false);
        srcC_tmp = fpu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x31:      // mmxadd R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "mmxadd R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(3);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x32:      // mmxadds R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "mmxadds R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(4);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x33:      // mmxsub R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "mmxsub R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(5);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x34:      // mmxsubs R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "mmxsubs R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(6);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x35:      // pmadd R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "pmadd R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(7);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x36:      // pack R1, R2, R3
        // LCOV_EXCL_BR_START
        cout << "pack R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(8);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0x37:      // mmxck R1, R2, R3 MMX Chroma Keying
        // LCOV_EXCL_BR_START
        cout << "mmxck R" << regC_tmp << ", R" << regA_tmp << "(=" << srcA_tmp
          << "), R" << regB_tmp << "(=" << srcB_tmp << ")\n";
        src_A.write(srcA_tmp);
        src_B.write(srcB_tmp);
        alu_op.write(9);
        mmx_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        while(mmx_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          mmx_valid_tmp = mmx_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        mmx_cs.write(false);
        srcC_tmp = mmx_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0xe0:      // flush register
        // LCOV_EXCL_BR_START
        cout << "flush\n";
        src_A.write(0);
        src_B.write(0);
        alu_op.write(0);
        rfu_rst.write(true);
        wait();
        rfu_rst.write(false);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0xf0:      // ldpid process_number
        // LCOV_EXCL_BR_START
        cout << "ldpid " <<  longlabel_tmp  << "\n";
        pid_data.write(sc_int<32>(longlabel_tmp));
        wait();
        pid_valid.write(false);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0xf1:	// movi R1, #value
        // LCOV_EXCL_BR_START
        cout << "movi R" << regC_tmp << ", " << imm_tmp << "\n";
        src_A.write(imm_tmp);
        src_B.write(0);
        alu_op.write(3);
        alu_cs.write(true);
        // LCOV_EXCL_BR_STOP
        wait();// LCOV_EXCL_BR_LINE
        alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        while(alu_valid_tmp == false){
          wait(); // LCOV_EXCL_BR_LINE
          alu_valid_tmp = alu_valid.read(); // LCOV_EXCL_BR_LINE
        }
        // LCOV_EXCL_BR_START
        alu_cs.write(false);
        srcC_tmp = alu_dout.read();
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      case 0xff:      // QUIT
        cout << "quit\n"; // LCOV_EXCL_BR_LINE
        while(true)
          wait(); // LCOV_EXCL_BR_LINE
        break;
      default :
        cout << "OPCODE "  << opcode_tmp  << " IS INVALID!\n"; // LCOV_EXCL_BR_LINE
        wait(); // LCOV_EXCL_BR_LINE
        break;
    }

    // Writeback
    switch(opcode_tmp)
    {
      case 0x01:	// add R1, R2, R3
      case 0x02:	// addi R1, R2, #value
      case 0x03:	// addc R1, R2, R3 + Carrybit
      case 0x04:	// sub R1, R2, R3
      case 0x05:	// subi R1, R2,  #value
      case 0x06:	// subc R1, R2, R3 - Carrybit
      case 0x07:	// mul R1, R2, R3
      case 0x08:	// div R1, R2, R3
      case 0x09:	// nand R1, R2, R3
      case 0x0b:	// or R1, R2, R3
      case 0x0c:	// xor R1, R2, R3
      case 0x0d:  // NOT R1, R2
      case 0x0e:  // modulo R1 = R2 mod R3
      case 0x4d:	// lw R1, R2, offset
      case 0x0f:	// mov R1, R2
      case 0x29:  // fadd R1, R2, R3
      case 0x2a:  // fsub R1, R2, R3
      case 0x2b:  // fmul R1, R2, R3
      case 0x2c:  // fdiv R1, R2, R3
      case 0x31:  // mmxadd R1, R2, R3
      case 0x32:  // mmxadds R1, R2, R3
      case 0x33:  // mmxsub R1, R2, R3
      case 0x34:  // mmxsubs R1, R2, R3
      case 0x35:  // pmadd R1, R2, R3
      case 0x36:  // pack R1, R2, R3
      case 0x37:  // mmxck R1, R2, R3 MMX Chroma Keying
      case 0xf1:	// movi R1, #value
        std::cout << "\t  R"<< regC_tmp << "=" << srcC_tmp << "\n"; // LCOV_EXCL_BR_LINE
        rfu_cs.write(true); // LCOV_EXCL_BR_LINE
        rfu_we.write(true); // LCOV_EXCL_BR_LINE
        rfu_addr3.write(regC_tmp); // LCOV_EXCL_BR_LINE
        rfu_data3.write(srcC_tmp); // LCOV_EXCL_BR_LINE
        wait(); // LCOV_EXCL_BR_LINE
        rfu_cs.write(false); // LCOV_EXCL_BR_LINE
        rfu_we.write(false); // LCOV_EXCL_BR_LINE
        wait(); // LCOV_EXCL_BR_LINE
        break;
      case 0x17:	// jal label for procedure call
        // LCOV_EXCL_BR_START
        rfu_cs.write(true);
        rfu_we.write(true);
        rfu_addr3.write(15);
        rfu_data3.write(pc.read()+1);
        wait();
        rfu_cs.write(false);
        rfu_we.write(false);
        wait();
        // LCOV_EXCL_BR_STOP
        break;
      default:
        wait(2); // LCOV_EXCL_BR_LINE
        break; } }
}  // LCOV_EXCL_BR_LINE

