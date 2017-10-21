//****************************************************
//  Crossroad example
//****************************************************

#include "systemc.h"
#include <math.h>

#ifndef _CROSSROAD_H_
#define _CROSSROAD_H_

enum direction {
  forward = 0,
  left = 1,
  right = 2,
  none = 3 // no car
};

//LCOV_EXCL_BR_START
SC_MODULE( crossroads )
{
  // Event for directions
  sc_event direction_changed;

  // Directions of cars from all 4 sides of crossroads
  direction x[4];

  // Main function for specific direction
  void drive (int id);

  // Forward direction handler
  void move_forward(int id);

  // Left direction handler
  void move_left(int id);

  // Process(es)
  void north();
  void east();
  void south();
  void west();

  // Constructor
  SC_CTOR( crossroads )
  {
    SC_THREAD(north);
    SC_THREAD(east);
    SC_THREAD(south);
    SC_THREAD(west);
  }
};
//LCOV_EXCL_BR_STOP

#endif
