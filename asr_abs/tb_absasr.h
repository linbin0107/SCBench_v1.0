#include <systemc.h>
#define LOOPCNT 20

SC_MODULE (test)
{
  sc_fifo_in<int> p_vl;
  sc_fifo_in<int> p_vr;
  sc_fifo_in<int> p_hl;
  sc_fifo_in<int> p_hr;

  sc_out<bool> inTicks_vl;
  sc_out<bool> inTicks_vr;
  sc_out<bool> inTicks_hl;
  sc_out<bool> inTicks_hr;

  void driver();
  void monitor_vl();
  void monitor_vr();
  void monitor_hl();
  void monitor_hr();

  void compare_results();

  SC_CTOR(test)
  {
    SC_THREAD(driver);

    SC_THREAD(monitor_vl);
    SC_THREAD(monitor_vr);
    SC_THREAD(monitor_hl);
    SC_THREAD(monitor_hr);
  }
};
