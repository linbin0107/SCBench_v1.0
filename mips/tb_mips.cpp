
#include "tb_mips.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void test::monitor()
{
  int res = 1;

  while (true)
  {
    while (isFinished.read() !=  true)
      wait();

    res = result.read();

    if (res == 0)
    {
      cout << "Finished simulation SUCCESSFULLY" << endl;
    } else {
      cout << "Finished simulation incorrectly" << endl;
    }

    sc_stop();

    wait();
  }
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
