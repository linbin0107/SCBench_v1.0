
#include "tb_simple_bus.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void simple_bus_test::compare_results()
{
  ifstream golden("bus_output_golden.txt");
  ifstream output("bus_output.txt");

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

    cout << "Finished simulation SUCCESSFULLY" << endl;

  } else {
    cout << "Could not open files" << endl;
  }

  golden.close();
  output.close();

}
//LCOV_EXCL_STOP
//LCOV_EXCL_BR_STOP
