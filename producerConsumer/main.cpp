//*****************************************
//  Producer-consumer example
//*****************************************

#include "producerConsumer.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[])
{
  sc_clock clk("clock", 10, SC_NS);

  proCon prodCons("ProcuderConsumer");
  prodCons.clk(clk);

  sc_start(500, SC_NS);

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
