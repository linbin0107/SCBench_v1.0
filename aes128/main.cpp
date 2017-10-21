#include <systemc.h>
#include <iostream>
#include "aes.h"
#include "tb_aes.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char* argv[])
{
	sc_signal<bool> reset;
	sc_signal<bool> rt_load;
	sc_signal<bool> rt_decrypt;
	sc_signal<sc_biguint<128> > rt_data_i;
	sc_signal<sc_biguint<128> > rt_key;

	sc_signal<sc_biguint<128> > rt_data_o;
	sc_signal<bool>rt_ready;

    sc_clock clk("clk", 10, SC_NS);

	aes *ae1 = new aes("aes");

	ae1->clk(clk);
	ae1->reset(reset);
	ae1->load_i(rt_load);
	ae1->decrypt_i(rt_decrypt);
	ae1->data_i(rt_data_i);
	ae1->key_i(rt_key);
	ae1->data_o(rt_data_o);
	ae1->ready_o(rt_ready);

	testing_module test_mod("tester");
	test_mod.clk(clk);
	test_mod.rst(reset);
	test_mod.load(rt_load);
	test_mod.decrypt(rt_decrypt);
	test_mod.data_o(rt_data_i);
	test_mod.key_o(rt_key);
	test_mod.data_i(rt_data_o);
	test_mod.ready_i(rt_ready);

#ifdef WAVE_DUMP
	sc_trace_file* trace_file = sc_create_vcd_trace_file("aes");

	sc_trace<bool>(trace_file, clk,"clk");
	sc_trace<bool>(trace_file,reset,"rst");
    sc_trace<sc_biguint<128> >(trace_file,ae1->addroundkey_data_o, "addroundkey_data_o");
	sc_trace<bool>(trace_file,rt_load,"load");
	sc_trace<bool>(trace_file,rt_decrypt, "decrypt");
	sc_trace<sc_biguint<128> >(trace_file,rt_data_i, "data_i");
	sc_trace<sc_biguint<128> >(trace_file,rt_key, "key");
	sc_trace<sc_biguint<128> >(trace_file,rt_data_o, "data_o");
	sc_trace<bool>(trace_file,rt_ready,"ready");
#endif

	sc_start(100, SC_US);

    test_mod.compare_results();

#ifdef WAVE_DUMP
	sc_close_vcd_trace_file(trace_file);
#endif

	return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
