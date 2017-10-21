//**********************************************
//  Producer-consumer example
//**********************************************

#include "producerConsumer.h"

// Producer
void proCon::producer() {
  wait();//LCOV_EXCL_BR_LINE

  while (*s) {
    c = *s;// No data race error
    s++;
    send.notify();//LCOV_EXCL_BR_LINE
    wait();//LCOV_EXCL_BR_LINE
  }

  c = *s;
  send.notify();//LCOV_EXCL_BR_LINE
  wait();
}

// Consumer
void proCon::consumer() {
  while (true) {
    // Waiting for a symbol
    wait(send); //LCOV_EXCL_BR_LINE

    char a = c;
    if (a == '\0') {
      break;
    }
  }
} //LCOV_EXCL_BR_LINE
