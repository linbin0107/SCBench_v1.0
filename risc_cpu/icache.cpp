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

  icache.cpp -- Instruction Cache Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Gene Bushuyev, Synopsys, Inc.
  Description of Modification: - bug - read/write outside of allocated memory

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: many code updates

 *****************************************************************************/


#include "systemc.h"
#include "icache.h"

#include <iostream>
using namespace std;

// LCOV_EXCL_BR_START
sc_uint<32> icache::icmemory[MAX_CODE_LENGTH] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0xf1e01000,	/* movi R14, 4096 * pc = 0x5=5 */
0xf1100800,	/* movi R1, 2048 * pc = 0x6=6 */
0xf1000000,	/* movi R0, 0 * pc = 0x7=7 */
0x0f200000,	/* mov R2, R0 * pc = 0x8=8 */
0x0f310000,	/* mov R3, R1 * pc = 0x9=9 */
0x13010009,	/* bge R0, R1, 9 * pc = 0xa=10 */
0x4e2e0000,	/* sw R2, R14, 0 * pc = 0xb=11 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0xc=12 */
0x4e3e0000,	/* sw R3, R14, 0 * pc = 0xd=13 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0xe=14 */
0x02220001,	/* addi R2, R2, 1 * pc = 0xf=15 */
0x05330001,	/* subi R3, R3, 1 * pc = 0x10=16 */
0x02000002,	/* addi R0, R0, 2 * pc = 0x11=17 */
0x1000fff8,	/* beq R0, R0, -8 * pc = 0x12=18 */
0x4d0e0001,	/* lw R0, R14, 1 * pc = 0x13=19 */
0x4d1e0002,	/* lw R1, R14, 2 * pc = 0x14=20 */
0x4d2e0003,	/* lw R2, R14, 3 * pc = 0x15=21 */
0x4d3e0004,	/* lw R3, R14, 4 * pc = 0x16=22 */
0x4d4e0005,	/* lw R4, R14, 5 * pc = 0x17=23 */
0x4d5e0006,	/* lw R5, R14, 6 * pc = 0x18=24 */
0x4d6e0007,	/* lw R6, R14, 7 * pc = 0x19=25 */
0x4d7e0006,	/* lw R7, R14, 6 * pc = 0x1a=26 */
0x4d8e0008,	/* lw R8, R14, 8 * pc = 0x1b=27 */
0x4d9e000a,	/* lw R9, R14, 10 * pc = 0x1c=28 */
0x4dae000b,	/* lw R10, R14, 11 * pc = 0x1d=29 */
0x4dbe000c,	/* lw R11, R14, 12 * pc = 0x1e=30 */
0x4dce000d,	/* lw R12, R14, 13 * pc = 0x1f=31 */
0x4dde000e,	/* lw R13, R14, 14 * pc = 0x20=32 */
0x00000000,	/* halt * pc = 0x21=33 */
0x0f0e0000,	/* mov R0, R14 * pc = 0x22=34 */
0x02000001,	/* addi R0, R0, 1 * pc = 0x23=35 */
0xf1100000,	/* movi R1, 0 * pc = 0x24=36 */
0xf12007ff,	/* movi R2, 2047 * pc = 0x25=37 */
0x4e2e0000,	/* sw R2, R14, 0 * pc = 0x26=38 */
0x05ee0001,	/* subi, R14, R14, 1  * pc = 0x27=39 */
0x4e1e0000,	/* sw R1, R14, 0 * pc = 0x28=40 */
0x05ee0001,	/* subi, R14, R14, 1 * pc = 0x29=41 */
0x4e0e0000,	/* sw R0, R14 * pc = 0x2a=42 */
0x1700003e,	/* jal 62 * pc = 0x2b=43 */
0x02ee0003,	/* addi R14, R14, 3 * pc = 0x2c=44 */
0x4d0e0000,	/* lw R0, R14, 0 * pc = 0x2d=45 */
0x4d1e0001,	/* lw R1, R14, 1 * pc = 0x2e=46 */
0x4d2e0002,	/* lw R2, R14, 2 * pc = 0x2f=47 */
0x4d3e0003,	/* lw R3, R14, 3 * pc = 0x30=48 */
0x4d4e0004,	/* lw R4, R14, 4 * pc = 0x31=49 */
0x4d5e0005,	/* lw R5, R14, 5 * pc = 0x32=50 */
0x4d6e0006,	/* lw R6, R14, 6 * pc = 0x33=51 */
0x4d7e0007,	/* lw R7, R14, 7 * pc = 0x34=52 */
0x4d8e0008,	/* lw R8, R14, 8 * pc = 0x35=53 */
0x4d9e0009,	/* lw R9, R14, 9 * pc = 0x36=54 */
0x4dae0000,	/* lw R10, R14,10 * pc = 0x37=55 */
0x4dbe000b,	/* lw R11, R14, 11 * pc = 0x38=56 */
0x4dce000c,	/* lw R12, R14, 12 * pc = 0x39=57 */
0x4dde000d,	/* lw R13, R14, 13 * pc = 0x3a=58 */
0x00000000,	/* halt * pc = 0x3b=59 */
0x16000005,	/* j 5 * pc = 0x3c=60 */
0xff000000,	/* quit * pc = 0x3d=61 */
0x4d0e0000,	/* lw R0, R14, 0 * pc = 0x3e=62 */
0x4d1e0001,	/* lw R1, R14, 1 * pc = 0x3f=63 */
0x4d2e0002,	/* lw R2, R14, 2 * pc = 0x40=64 */
0x00000000,	/* halt * pc = 0x41=65 */
0x0f310000,	/* mov R3, R1  * pc = 0x42=66 */
0x0f420000,	/* mov R4, R2 * pc = 0x43=67 */
0x01612000,	/* add R6, R1, R2 * pc = 0x44=68 */
0xf1700002,	/* movi R7, 2 * pc = 0x45=69 */
0x08667000,	/* div R6, R6, R7 * pc = 0x46=70 */
0x01606000,	/* add R6, R0, R6 * pc = 0x47=71 */
0x4d660000,	/* lw R6, R6, 0 * pc = 0x48=72 */
0x12340017,	/* bgt R3, R4, 23 * pc = 0x49=73 */
0x01703000,	/* add R7, R0, R3 * pc = 0x4a=74 */
0x4d770000,	/* lw R7, R7, 0 * pc = 0x4b=75 */
0x13760003,	/* bge R7, R6, 3 * pc = 0x4c=76 */
0x02330001,	/* addi R3, R3, 1 * pc = 0x4d=77 */
0x1000fffc,	/* beq R0, R0, -4 * pc = 0x4e=78 */
0x01704000,	/* add R7, R0, R4 * pc = 0x4f=79 */
0x4d770000,	/* lw R7, R7, 0 * pc = 0x50=80 */
0x15760003,	/* ble R7, R6 3 * pc = 0x51=81 */
0x05440001,	/* subi, R4, R4, 1 * pc = 0x52=82 */
0x1000fffc,	/* beq R0, R0, -4 * pc = 0x53=83 */
0x1234000b,	/* bgt R3, R4, 11 * pc = 0x54=84 */
0x01503000,	/* add R5, R0, R3 * pc = 0x55=85 */
0x4d550000,	/* lw R5, R5, 0 * pc = 0x56=86 */
0x01704000,	/* add R7, R0, R4 * pc = 0x57=87 */
0x4d770000,	/* lw R7, R7, 0 * pc = 0x58=88 */
0x01803000,	/* add R8, R0, R3 * pc = 0x59=89 */
0x4e780000,	/* sw R7, R8, 0 * pc = 0x5a=90 */
0x01704000,	/* add R7, R0, R4 * pc = 0x5b=91 */
0x4e570000,	/* sw R5, R7, 0 * pc = 0x5c=92 */
0x02330001,	/* addi R3, R3, 1 * pc = 0x5d=93 */
0x05440001,	/* subi R4, R4, 1 * pc = 0x5e=94 */
0x1000ffea,	/* beq R0, R0, -22 * pc = 0x5f=95 */
0x13140021,	/* bge R1, R4, 33 * pc = 0x60=96 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x61=97 */
0x4efe0000,	/* sw R15, R14, 0 * pc = 0x62=98 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x63=99 */
0x4e0e0000,	/* sw R0, R14, 0 * pc = 0x64=100 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x65=101 */
0x4e1e0000,	/* sw R1, R14, 0 * pc = 0x66=102 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x67=103 */
0x4e2e0000,	/* sw R2, R14, 0 * pc = 0x68=104 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x69=105 */
0x4e3e0000,	/* sw R3, R14, 0 * pc = 0x6a=106 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x6b=107 */
0x4e4e0000,	/* sw R4, R14, 0 * pc = 0x6c=108 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x6d=109 */
0x4e4e0000,	/* sw R4, R14, 0 * pc = 0x6e=110 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x6f=111 */
0x4e1e0000,	/* sw R1, R14, 0 * pc = 0x70=112 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x71=113 */
0x4e0e0000,	/* sw R0, R14, 0 * pc = 0x72=114 */
0x1700003f,	/* jal 63 * pc = 0x73=115 */
0x02ee0003,	/* addi, R14, R14, 3 * pc = 0x74=116 */
0x4d4e0000,	/* lw R4, R14, 0 * pc = 0x75=117 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x76=118 */
0x4d3e0000,	/* lw R3, R14, 0 * pc = 0x77=119 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x78=120 */
0x4d2e0000,	/* lw R2, R14, 0 * pc = 0x79=121 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x7a=122 */
0x4d1e0000,	/* lw R1, R14, 0 * pc = 0x7b=123 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x7c=124 */
0x4d0e0000,	/* lw R0, R14, 0 * pc = 0x7d=125 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x7e=126 */
0x4dfe0000,	/* lw R15, R14, 0 * pc = 0x7f=127 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x80=128 */
0x13230021,	/* bge R2, R3, 33 * pc = 0x81=129 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x82=130 */
0x4efe0000,	/* sw R15, R14, 0 * pc = 0x83=131 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x84=132 */
0x4e0e0000,	/* sw R0, R14, 0 * pc = 0x85=133 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x86=134 */
0x4e1e0000,	/* sw R1, R14, 0 * pc = 0x87=135 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x88=136 */
0x4e2e0000,	/* sw R2, R14, 0 * pc = 0x89=137 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x8a=138 */
0x4e3e0000,	/* sw R3, R14, 0 * pc = 0x8b=139 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x8c=140 */
0x4e4e0000,	/* sw R4, R14, 0 * pc = 0x8d=141 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x8e=142 */
0x4e2e0000,	/* sw R2, R14, 0 * pc = 0x8f=143 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x90=144 */
0x4e3e0000,	/* sw R3, R14, 0 * pc = 0x91=145 */
0x05ee0001,	/* subi R14, R14, 1 * pc = 0x92=146 */
0x4e0e0000,	/* sw R0, R14, 0 * pc = 0x93=147 */
0x1700003f,	/* jal 63 * pc = 0x94=148 */
0x02ee0003,	/* addi, R14, R14, 3 * pc = 0x95=149 */
0x4d4e0000,	/* lw R4, R14, 0 * pc = 0x96=150 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x97=151 */
0x4d3e0000,	/* lw R3, R14, 0 * pc = 0x98=152 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x99=153 */
0x4d2e0000,	/* lw R2, R14, 0 * pc = 0x9a=154 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x9b=155 */
0x4d1e0000,	/* lw R1, R14, 0 * pc = 0x9c=156 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x9d=157 */
0x4d0e0000,	/* lw R0, R14, 0 * pc = 0x9e=158 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0x9f=159 */
0x4dfe0000,	/* lw R15, R14, 0 * pc = 0xa0=160 */
0x02ee0001,	/* addi R14, R14, 1 * pc = 0xa1=161 */
0x18000000,	/* jr * pc = 0xa2=162 */
0xffffffff
};
// LCOV_EXCL_BR_STOP

void icache::entry()
{
  bool cs_tmp = false;
  bool ld_valid_tmp = false;
  sc_uint<32>  address;// LCOV_EXCL_BR_LINE

  wait();

  while (true)
  {
    wait(); //LCOV_EXCL_BR_LINE
    cs_tmp = cs.read(); //LCOV_EXCL_BR_LINE
    while(cs_tmp != true) {
      wait(); //LCOV_EXCL_BR_LINE
      cs_tmp = cs.read(); //LCOV_EXCL_BR_LINE
    }

    address = addr.read(); //LCOV_EXCL_BR_LINE

    if (address >= BOOT_LENGTH-1)
    {
      ld_valid_tmp = ld_valid.read(); //LCOV_EXCL_BR_LINE
      if (ld_valid_tmp == true)
      {
        // LCOV_EXCL_BR_START
        pid = ld_data.read();
        wait();
        wait();
        // LCOV_EXCL_BR_STOP
      }

      // Introduce delay needed
      wait(wait_cycles); // LCOV_EXCL_BR_LINE

      if (address >= MAX_CODE_LENGTH || address < 0)
      {
        // LCOV_EXCL_BR_START
        dataout.write(0xffffffff);
        std::cout << "ICU ALERT: **MEMORY OUT OF RANGE (PC = "<< address << ") **\n";
        // LCOV_EXCL_BR_STOP
      }
      else
        dataout.write(icmemory[address]); //LCOV_EXCL_BR_LINE
      // LCOV_EXCL_BR_START
      icache_valid.write(true);
      wait();
      icache_valid.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    }
  }// LCOV_EXCL_LINE
} //LCOV_EXCL_BR_LINE

