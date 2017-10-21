#include <cstdlib>
#include <stdint.h>
#include "tb_exec.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void tb_exec::driver() {
  uint8_t opcode;
  int dina, dinb;
  bool cs;

  wait(3);
  cs_out.write(0);
  wait();
  cs_out.write(1);
  wait();

  while(1) {
    while (isBusy.read() == true)
      wait();

    opcode_out.write(0xa);
    dina_out.write(0x625558ec);
    dinb_out.write(0x238e1f29);

    do {
      wait();
    } while (isBusy.read() == true);

    opcode_out.write(0x9);
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

void tb_exec::monitor() {
  ofstream ofs;
  ofs.open("exec_output.txt");

  ofs << "Time result carry overflow zero" << endl;
  ofs.close();

  wait();

  while (true) {
    while (isValid.read() != true)
      wait();

    ofstream ofs;
    ofs.open("exec_output.txt", ios::out | ios::app);

    ofs << sc_time_stamp() << hex << "  0x" << result.read() <<
        dec << "  " << carry.read() << "  " << overflow.read() <<
        "  " << zero.read() << endl;

    ofs.close();

    wait();
  }
}

void tb_exec::compare_results() {
  ifstream golden("exec_output_golden.txt");
  ifstream output("exec_output.txt");

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
