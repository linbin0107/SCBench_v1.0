
#include "mips.h"
#include "tb_mips.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[]){
  sc_signal<int> result;
  sc_signal<bool> finished;

  sc_clock clk("clock", 10, SC_NS);

  mips U_mips("mips");
  U_mips.clk(clk);
  U_mips.finished(finished);
  U_mips.out(result);

  test test_mod("test");
  test_mod.clk(clk);
  test_mod.isFinished(finished);
  test_mod.result(result);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("mips");

  sc_trace(trace_file, clk, "clk");
  sc_trace(trace_file, result, "result");
  sc_trace(trace_file, finished, "finished");
#endif

  sc_start();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
