#include "tb_aes.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void testing_module::driver_monitor()
{
	sc_biguint<128> data =  91666365;
	while(true)
	{
	  rst = true;
	  load = 0;
	  decrypt = 0;
	  data_o = 0;
	  key_o = 0;
	  wait();

	  // encrypt
	  load = true;
	  data_o = data;
	  key_o = 14249380;
	  decrypt= 0;
	  do
	  {
		wait();
	  } while(!clk.posedge());

	  load = false;

	  do
	  {
		wait();
      } while( ready_i.read() != true);

	  assert(data != data_i.read());

	  sc_biguint<128> next_data = data_i;
	  // decrypt
	  load = true;
	  data_o = data_i;
	  decrypt = true;

	  do
	  {
		wait();
	  } while(!clk.posedge());

	  load = false;

	  do
	  {
		wait();
	  } while( ready_i.read() != true);

	  assert(data == data_i.read());
	  data = next_data;

      ofstream os;
      os.open("aes_output.txt", ios::out | ios::app);
      os << data;
      os << "\n";
      os.close();

	  std::cout <<  data.range(127,96) << "'"
		  << data.range(95,64)  <<"'"
		  << data.range(63,32) <<"'"
		  <<data.range(31,0) << std::endl;
	}

}

void testing_module::compare_results()
{
  ifstream golden("aes_output_golden.txt");
  ifstream output("aes_output.txt");

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
