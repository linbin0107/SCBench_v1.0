//**********************************************
//  Crossroad example
//**********************************************

#include "crossroad.h"

void crossroads::drive (int id) {
   while (true) {
     // Generate direction
     // Data race error (Read-Write) with lines 15, 34, 45
     x[id] = (direction) (random() % 4);
     direction_changed.notify(0, SC_NS);//LCOV_EXCL_BR_LINE
     wait(0, SC_NS);//LCOV_EXCL_BR_LINE

     switch(x[id]) {
       case forward:
   	     move_forward(id);
   	     break;
       case left:
   	     move_left(id);
   	     break;
       case right:
       case none:
   	     ; // nothing to do
     }
   }
}

// Forward direction handler
void crossroads::move_forward(int id) {
  int right_index = (id - 1 + 4) % 4;

  // waiting for none right car
  while (x[right_index] != none) {
    wait(direction_changed);//LCOV_EXCL_BR_LINE
  }
}

// Left direction handler
void crossroads::move_left(int id) {
  int right_index = (id - 1 + 4) % 4;
  int forward_index = (id + 2) % 4;

  // waiting for none right car and none forward car
  while (x[right_index] != none || x[forward_index] != none) {
    wait(direction_changed);//LCOV_EXCL_BR_LINE
  }
}

// Process(es)
void crossroads::north() {
  drive(0);
}

void crossroads::east() {
  drive(1);
}
void crossroads::south() {
  drive(2);
}
void crossroads::west() {
  drive(3);
}
