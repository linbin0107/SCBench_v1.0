#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(testt)
{
  sc_in<bool> clk;
  sc_out<bool> reset;
  sc_out<sc_uint<32> > bus_out;

  void test();

  SC_CTOR(testt) {
   SC_CTHREAD(test, clk.pos());
  }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
