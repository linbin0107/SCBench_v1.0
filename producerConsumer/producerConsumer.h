//********************************************
//  Producer-consumer example
//********************************************

#ifndef _PRODUCERCONSUMER_H_
#define _PRODUCERCONSUMER_H_

#include "systemc.h"

//LCOV_EXCL_BR_START
SC_MODULE(proCon)
{
  sc_in<bool> clk;

  char* s;
  char c;
  sc_event send;

  // Producer
  void producer();

  // Consumer
  void consumer();

  // constructor
  SC_CTOR(proCon)
  {
    s = (char*)"Hello world and exit";
    c = 0;

    SC_THREAD(producer);
    sensitive << clk.pos();
    SC_THREAD(consumer);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP

#endif
