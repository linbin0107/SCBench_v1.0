#include "tb_des.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void testert::driver_monitor()
{
  sc_uint<64> data;
  data = 0xdeadbeef;
  data <<= 32;
  data |= 0xdeadbeef;

  sc_uint<64> k;
  k = 0xfb16c632;
  k <<= 32;
  k |= 0xab844fd8;
  key = k;

  reset = true;

  while(true)
  {
    // encrypt
    load = true;
    data_o = data;
    decrypt = false;

    do {
	  wait();
    } while(!clk.posedge());

    assert(data != data_i.read());

    load = false;

    do {
	  wait();
    } while( ready.read() != true);

    sc_uint<64> next_data = data_i;

    // decrypt
    load = true;
    data_o = data_i;
    decrypt = true;

    do {
	  wait();
    } while(!clk.posedge());

    load = false;

    do {
	  wait();
    } while( ready.read() != true);

    assert(data == data_i.read());
    data = next_data;

    ofstream os;
    os.open("des_output.txt", ios::out | ios::app);
    os << data;
    os << "\n";
    os.close();

    std::cout << data.range(63,32) << "'"
       << data.range(31,0) << std::endl;
  }
}

void testert::compare_results()
{
  ifstream golden("des_output_golden.txt");
  ifstream output("des_output.txt");

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
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
