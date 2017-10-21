#include <systemc.h>
#include "tb_absasr.h"

#define LOOPCNT 20

void test::driver()
{
  bool sw = false;

  wait(1, SC_SEC);

  for(int i = 0; i < LOOPCNT; i = i + 1) //  ca 7 sec
  {
    wait(6800, SC_US); // -> 200 km/h
    inTicks_vl.write(sw);
    inTicks_vr.write(sw);
    inTicks_hl.write(sw);
    inTicks_hr.write(sw);
    sw = !sw;
  }

  wait(2, SC_SEC);
}

void test::monitor_vl(){
  int p;
  int last_p;

  while(true){
	p = p_vl.read();
	if(p != last_p) {
      ofstream os;
      os.open("absasr_output.txt", ios::out | ios::app);
	  os << sc_time_stamp() << " pressure at left front wheel: " << p << endl;
      os.close();
    }

    last_p = p;
  }
}

void test::monitor_vr(){
  int p;
  int last_p;

  while(true){
    p = p_vr.read();
    if(p != last_p) {
      ofstream os;
      os.open("absasr_output.txt", ios::out | ios::app);
      os << sc_time_stamp() << " pressure at right front wheel: " << p << endl;
      os.close();
    }

	last_p = p;
  }
}

void test::monitor_hl(){
  int p;
  int last_p;

  while(true){
    p = p_hl.read();
    if(p != last_p) {
      ofstream os;
      os.open("absasr_output.txt", ios::out | ios::app);
      os << sc_time_stamp() << " pressure at left rear wheel: " << p << endl;
      os.close();
    }

	last_p = p;
  }
}

void test::monitor_hr(){
  int p;
  int last_p;

  while(true){
	p =  p_hr.read();
	if(p != last_p) {
      ofstream os;
      os.open("absasr_output.txt", ios::out | ios::app);
      os << sc_time_stamp() << " pressure at right rear wheel: " << p << endl;
      os.close();
    }

	last_p = p;
  }
}

void test::compare_results() {
  ifstream golden("absasr_output_golden.txt");
  ifstream output("absasr_output.txt");

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
