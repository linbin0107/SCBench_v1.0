//*****************************************************************************
//
//  Dining philosophers example
//
//*****************************************************************************

#include "philosophers.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[]) {
  sc_clock clk("clk", 10, SC_NS);

  Philosophers phi("philosophers");
  phi.clk(clk);

  sc_start(300, SC_NS);

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
