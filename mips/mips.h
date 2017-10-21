
#ifndef _MIPS_H_
#define _MIPS_H_

#include <stdio.h>
#include "systemc.h"


#define R 0

#define ADDU 33
#define SUBU 35

#define MULT 24
#define MULTU 25

#define MFHI 16
#define MFLO 18

#define AND 36
#define OR 37
#define XOR 38
#define SLL 0
#define SRL 2
#define SLLV 4
#define SRLV 6

#define SLT 42
#define SLTU 43

#define JR 8

#define J 2
#define JAL 3

#define ADDIU 9
#define ANDI 12
#define ORI 13
#define XORI 14

#define LW 35
#define SW 43
#define LUI 15

#define BEQ 4
#define BNE 5
#define BGEZ 1

#define SLTI 10
#define SLTIU 11

const int A[8] = { 22, 5, -9, 3, -17, 38, 0, 11 };
const int outData[8] = { -17, -9, 0, 3, 5, 11, 22, 38 };

#define IADDR(x)	(((x)&0x000000ff)>>2)
#define DADDR(x)	(((x)&0x000000ff)>>2)

// LCOV_EXCL_BR_START
SC_MODULE(mips) {
  sc_in<bool> clk;

  sc_out<bool> finished;
  sc_out<int> out;

  void run();

  SC_CTOR(mips){
    SC_CTHREAD(run, clk.pos());
  }
};
// LCOV_EXCL_BR_STOP
#endif
