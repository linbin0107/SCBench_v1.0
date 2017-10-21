
#ifndef _TB_MIPS_H_
#define _TB_MIPS_H_

#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(test) {
  sc_in_clk clk;
  sc_in<bool> isFinished;
  sc_in<int> result;

  void monitor();

  SC_CTOR(test) {
    SC_THREAD(monitor);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP

#endif
