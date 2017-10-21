#include <stdint.h>
#include "tb_Y86.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
uint32_t program[] = {
0x0000F629,
0x0000C029,
0x0017568B,
0x0000D001,
0x0000C301,
0x0000C189,
0x001B568B,
0x0000D129,
0x0000F075,
0x000000F4,
0x00000001,
0x0000000A,
};

void testt::test(){
  unsigned int bus;

  reset.write(1);
  wait();
  reset.write(0);

  wait();
  while(true){
    for(int i = 0; i < 12; i++){
      bus_out.write(program[i]);
      wait();
    }

    sc_stop();
  }
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
