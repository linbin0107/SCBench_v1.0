/*   This is a simple Anti-lock Braking System. The inputs are the
//   number of ticks counted within 100 ms at each wheel. Outputs are
//   control signals for pressure control (0 - constant pressure, 2 -
//   increase pressure, -1 - decrease pressure).
*/

#include <systemc.h>
#include "settings.h"


SC_MODULE(absasr){

//-*-*-*-*-*-*-*-*-*-*-*-*  INTERFACE  *-*-*-*-*-*-*-*-*-*-*-*-

  // tick counter inputs
  sc_fifo_in<int> bus_vl;
  sc_fifo_in<int> bus_vr;
  sc_fifo_in<int> bus_hl;
  sc_fifo_in<int> bus_hr;

  // pressure outputs
  sc_fifo_out<int> pr_vl;
  sc_fifo_out<int> pr_vr;
  sc_fifo_out<int> pr_hl;
  sc_fifo_out<int> pr_hr;

//-*-*-*-*-*-*-*-*-*-*-*-* LOKALE VARIABLEN *-*-*-*-*-*-*-*-*-*-*-*-

// current speed v and acceleration a at all four wheels
  int v[4];
  int a[4];
  int v_last[4];
// estimated car speed
  int temp_fv;
  int fv;
// estimated car acceleration
  int fa;
// slack at all four wheels
  int lambda[4];
// state of the ABS control cycle at all four wheels
  int s[4];
// temporary variables for pressure and for acceleration calculation
  int p[4];
  int tmp[4];


  // receive ticks from right front weel and compute speed and acceleration
  void read_vr();

  // receive ticks from left front weel and compute speed and acceleration
  void read_vl();

  // receive ticks from right rear weel and compute speed and acceleration
  void read_hr();

  // receive ticks from left rear weel and compute speed and acceleration
  void read_hl();

  // ABS control - main routine
  void ABS();

  // ASR control - main routine
  void ASR();

  int abs(int val);

  // main thread, triggers ASR or ABS control if necessary
  void run();

//-*-*-*-*-*-*-*-*-*-*-*-*-*-* CONSTRUCTOR *-*-*-*-*-*-*-*-*-*-*-*-*-*-

  SC_CTOR(absasr){
    SC_THREAD(run);
  }

};


