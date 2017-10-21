//********************************************
//  Dining philosophers example
//********************************************

#include "philosophers.h"

void Philosophers::Eating() {
  wait(rand()%3 + 1);//LCOV_EXCL_BR_LINE
}

void Philosophers::Thinking() {
  wait(rand()%3 + 1);//LCOV_EXCL_BR_LINE
}

// Philosopher actions
void Philosophers::philosopher (int id) {
  while (true) {
    Thinking();

    // Try to take left fork
    while (forks[id]) {// Data race error (Read-Write) with lines 26 and 37
      wait(fork_takeoff);//LCOV_EXCL_BR_LINE
    }

    // Take left fork
    forks[id] = 1;// Data race error (Write-Write) with line 37
    // Right fork is taken next clock
    wait();//LCOV_EXCL_BR_LINE

    // Try to take right fork
    while (forks[(id+1)%PHN]) {
      wait(fork_takeoff);//LCOV_EXCL_BR_LINE
    }

    // Take right fork
    forks[(id+1)%PHN] = 1;

    Eating();

    // Take off both forks
    forks[id] = 0;
    forks[(id+1)%PHN] = 0;

    fork_takeoff.notify(SC_ZERO_TIME);//LCOV_EXCL_BR_LINE
    wait();//LCOV_EXCL_BR_LINE
  }
}

// Process(es)
void Philosophers::phil1() {
  int id = 0;
  philosopher(id);
}

void Philosophers::phil2() {
  int id = 1;
  philosopher(id);
}

void Philosophers::phil3() {
  int id = 2;
  philosopher(id);
}

void Philosophers::phil4() {
  int id = 3;
  philosopher(id);
}

void Philosophers::phil5() {
  int id = 4;
  philosopher(id);
}

void Philosophers::phil6() {
  int id = 5;
  philosopher(id);
}

void Philosophers::phil7() {
  int id = 6;
  philosopher(id);
}

void Philosophers::phil8() {
  int id = 7;
  philosopher(id);
}

void Philosophers::phil9() {
  int id = 8;
  philosopher(id);
}

void Philosophers::phil10() {
  int id = 9;
  philosopher(id);
}

