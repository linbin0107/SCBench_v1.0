#include <systemc.h>

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(testert)
{
  sc_in <bool> clk;

  sc_out < bool > reset;
  sc_out < bool > load;
  sc_out < bool > decrypt;
  sc_out < sc_uint < 64 > > data_o;
  sc_out < sc_uint < 64 > >key;

  sc_in < sc_uint < 64 > > data_i;
  sc_in < bool > ready;

  void compare_results();

  void driver_monitor();

  SC_CTOR(testert)
  {
	  SC_CTHREAD(driver_monitor,clk.pos());
  }

};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
