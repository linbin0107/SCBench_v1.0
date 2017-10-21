//**********************************************
//  Dining philosophers example
//**********************************************

#include "systemc.h"
#include <math.h>

#ifndef _PHILOSOPHERS_H_
#define _PHILOSOPHERS_H_

// Philosopher number
const int PHN = 10;

//LCOV_EXCL_BR_START
SC_MODULE(Philosophers) {
  sc_in<bool> clk;

  // Event for philosopher takes off a fork
  sc_event fork_takeoff;

  // Forks usage flags
  int forks[PHN];

  void Eating();

  void Thinking();

  // Philosopher actions
  void philosopher (int id);

  // Process(es)
  void phil1();
  void phil2();
  void phil3();
  void phil4();
  void phil5();
  void phil6();
  void phil7();
  void phil8();
  void phil9();
  void phil10();

  // Constructor
  SC_CTOR(Philosophers) {
    // Forks are unused
    int i;
    for (i = 0; i < PHN; i++) {
        forks[i] = 0;
    }

    SC_THREAD( phil1);
    sensitive << clk.pos();
    SC_THREAD( phil2 );
    sensitive << clk.pos();
    SC_THREAD( phil3 );
    sensitive << clk.pos();
    SC_THREAD( phil4 );
    sensitive << clk.pos();
    SC_THREAD( phil5 );
    sensitive << clk.pos();
    SC_THREAD( phil6 );
    sensitive << clk.pos();
    SC_THREAD( phil7 );
    sensitive << clk.pos();
    SC_THREAD( phil8 );
    sensitive << clk.pos();
    SC_THREAD( phil9 );
    sensitive << clk.pos();
    SC_THREAD( phil10 );
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP

#endif
