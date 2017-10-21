#include <systemc.h>

#include "TickCounter.h"
#include "absasr.h"
#include "tb_absasr.h"
//#include "outputmon.h"

int sc_main (int argc, char* argv[]){

  // ticks from input generator
  sc_signal<bool> ch_inTicks_vl;
  sc_signal<bool> ch_inTicks_vr;
  sc_signal<bool> ch_inTicks_hl;
  sc_signal<bool> ch_inTicks_hr;

  // pressure to output monitor
  sc_fifo<int> d_vl;
  sc_fifo<int> d_vr;
  sc_fifo<int> d_hl;
  sc_fifo<int> d_hr;

  // ticks from tick counter to ECU
  sc_fifo<int> ticks_vl;
  sc_fifo<int> ticks_vr;
  sc_fifo<int> ticks_hl;
  sc_fifo<int> ticks_hr;

  // instantiate modules and connect channels

  test tt("test");
  tt.inTicks_vl(ch_inTicks_vl);
  tt.inTicks_vr(ch_inTicks_vr);
  tt.inTicks_hl(ch_inTicks_hl);
  tt.inTicks_hr(ch_inTicks_hr);
  tt.p_vl(d_vl);
  tt.p_hl(d_hl);
  tt.p_vr(d_vr);
  tt.p_hr(d_hr);

  TickCounter vl("vl");
  vl.inTicks(ch_inTicks_vl);
  vl.outTicks(ticks_vl);

  TickCounter vr("vr");
  vr.inTicks(ch_inTicks_vr);
  vr.outTicks(ticks_vr);

  TickCounter hl("hl");
  hl.inTicks(ch_inTicks_hl);
  hl.outTicks(ticks_hl);

  TickCounter hr("hr");
  hr.inTicks(ch_inTicks_hr);
  hr.outTicks(ticks_hr);

  absasr ecu_absasr("absasr");
  ecu_absasr.bus_vl(ticks_vl);
  ecu_absasr.bus_vr(ticks_vr);
  ecu_absasr.bus_hl(ticks_hl);
  ecu_absasr.bus_hr(ticks_hr);
  ecu_absasr.pr_vl(d_vl);
  ecu_absasr.pr_vr(d_vr);
  ecu_absasr.pr_hl(d_hl);
  ecu_absasr.pr_hr(d_hr);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("simple_abs");

  sc_trace(trace_file, ch_inTicks_vl, "ch_inTicks_vl");
  sc_trace(trace_file, ch_inTicks_vr, "ch_inTicks_vr");
  sc_trace(trace_file, ch_inTicks_hl, "ch_inTicks_hl");
  sc_trace(trace_file, ch_inTicks_hr, "ch_inTicks_hr");
#endif

  // run the simulation till sc_stop is encountered
  sc_start(5,SC_SEC);

  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;

  tt.compare_results();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  // terminate simulation
  return 0;

}
