#include <systemc.h>

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(testing_module)
{
	sc_in<bool> clk;
	sc_inout<bool> rst;

	sc_inout<bool> load;
	sc_inout<bool> decrypt;
	sc_inout<sc_biguint<128> > data_o;
	sc_inout<sc_biguint<128> > key_o;

	sc_in<sc_biguint<128> > data_i;
	sc_in<bool > ready_i;

    void compare_results();

	void driver_monitor();

	SC_CTOR(testing_module)
	{
		SC_CTHREAD(driver_monitor, clk.pos());
	}
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
