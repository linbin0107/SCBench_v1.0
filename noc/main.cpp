#include <systemc>
#include <iostream>
#include <stdint.h>
#include "top.h"
#include "power_model.h"

using namespace sc_core;

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
double oi_energy = 0;
double ei_energy = 0;
double oi_delay = 0;
double ei_delay = 0;

int sc_main(int argc, char *argv[])
{
    uint32_t column = 0, row = 0;
    uint8_t depth = 0;
    FILE *in_file;

    in_file = fopen("noc_input.bin", "rb");
    assert(in_file);

    fread(&column, sizeof(int32_t), 1, in_file);
    fread(&row, sizeof(int32_t), 1, in_file);
    fread(&depth, sizeof(uint8_t), 1, in_file);

    fclose(in_file);

	cout << "---------------------------------------------" << endl;
	cout << "ROW: [" << row << "], COLUMN: [" << column << "]"<< endl;
	cout << "FIFO DEPTH: [" << (uint32_t) depth << "]"<< endl;
	sc_set_time_resolution(1, SC_NS);

	sc_clock clk("clk", 10, SC_NS);

	top noc("noc", column, row, depth);
	noc.clk(clk);

#ifdef WAVE_DUMP
	sc_trace_file *trace_file;

	trace_file = sc_create_vcd_trace_file("noc");
	trace_file->set_time_unit(1, SC_NS);

	sc_trace(trace_file, noc.clk, "clk");
	sc_trace(trace_file, noc.reset_n, "reset_n");

	//Signal between tile1 and tile2.
 	sc_trace(trace_file, noc.data_in[0], "data_in0");
	sc_trace(trace_file, noc.data_out[0], "data_out0");
	sc_trace(trace_file, noc.oi_in[0], "OI_in0");
	sc_trace(trace_file, noc.oi_out[0], "OI_out0");

	sc_trace(trace_file, noc.in_full[0],"in_full0");
	sc_trace(trace_file, noc.out_full[0], "out_full0");
	sc_trace(trace_file, noc.in_sel[0], "in_sel0");
	sc_trace(trace_file, noc.out_sel[0], "out_sel0");
	sc_trace(trace_file, noc.in_write_n[0], "in_write_n0");
	sc_trace(trace_file, noc.out_write_n[0], "out_write_n0");
#endif

	sc_start(5000, SC_NS);

#ifdef WAVE_DUMP
	sc_close_vcd_trace_file(trace_file);
#endif

	Energy_disp();

	return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
