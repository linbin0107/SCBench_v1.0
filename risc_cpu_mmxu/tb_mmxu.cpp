#include <cstdlib>
#include <stdint.h>
#include "tb_mmxu.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void tb_mmxu::driver() {
  uint8_t opcode;
  bool cs;
  int dina, dinb;

  wait(3);
  cs_out.write(0);
  wait();
  cs_out.write(1);
  wait();

  while(1) {
    while (isBusy.read() == true)
      wait();

    opcode_out.write(3);
    dina_out.write(0x352d7aad);
    dinb_out.write(0x2f019afd);

    do {
      wait();
    } while (isBusy.read() == true);

    opcode_out.write(7);
    dina_out.write(0xdeadbeef);
    dinb_out.write(0xbeefdead);

    do {
      wait();
    } while (isBusy.read() == true);


    cs_out.write(0);
    wait(3);

    compare_results();
    sc_stop();
  }
}

void tb_mmxu::monitor() {
  ofstream ofs;
  ofs.open("mmxu_output.txt");

  ofs << "Time   result" << endl;

  ofs.close();

  wait();

  while (true) {
    while (isValid.read() != true)
      wait();

    ofstream ofs;
    ofs.open("mmxu_output.txt", ios::out | ios::app);

    ofs << sc_time_stamp() << hex << " 0x" << result.read() << endl;

    ofs.close();

    wait();
  }

}

void tb_mmxu::compare_results() {
  ifstream golden("mmxu_output_golden.txt");
  ifstream output("mmxu_output.txt");

  if (golden.is_open() && output.is_open())
  {
    std::string g_line, o_line;

    while (getline(golden, g_line))
    {
      if (getline(output, o_line))
      {
        if (g_line.compare(o_line) != 0)
          cout << "\nFinished simulation incorrectly" << endl;
      }

    }

    cout << "\nFinished simulation SUCCESSFULLY" << endl;

  } else {
    cout << "Could not open files" << endl;
  }

  golden.close();
  output.close();
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
