/***************************************************
 * File: tb_floating.cpp
 * Description: testbench file for floating
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 09/01/2016
 **************************************************/

#include "tb_floating.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_STOP
void tb_floating::driver()
{
  int dina_tmp;
  int dinb_tmp;

  cs.write(false);
  wait();
  cs.write(true);
  opcode.write(0);
  wait(7);

  while (true)
  {
    opcode.write(3);
    dina.write(0xdeadbeef);
    dinb.write(0xbeefdead);
    wait(4);

    opcode.write(6);
    wait(4);

    opcode.write(5);
    wait(7);

    compare_results();

    sc_stop();
  }
}

void tb_floating::monitor()
{
  wait();

  while (true)
  {
    while (isValid.read() != true)
    {
      wait();
    }

    ofstream ofs("floating_output.txt", ios::out | ios::app);
    ofs << hex <<"0x" << result.read() << "\n";
    ofs.close();

    cout << hex << "result: " << result.read() << endl;

    //sc_stop();

    wait();
  }
}

void tb_floating::compare_results()
{
  ifstream golden("floating_output_golden.txt");
  ifstream output("floating_output.txt");

  if (golden.is_open() && output.is_open())
  {
    std::string g_line, o_line;

    while (getline(golden, g_line))
    {
      if (getline(output, o_line))
      {
        if (g_line.compare(o_line) != 0)
          cout << "Finished simulation incorrectly" << endl;
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
