
#include "data.h"
#include "defines.h"
#include "ahb_simple_mem.h"
//#include "ahb_to_tlm2_slave.h"
#include "tlm2_master_to_ahb.h"
#include "tlm2_simple_master.h"
//#include "tlm2_simple_slave.h"
#include "ahb_simple_bus.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char* argv[])
{
  /* TODO mp: in the original model we are sensitive only
  * to the posedge of a clock with 10 SC_NS. This should be
  * the same as being sensitive to all edges of a clock with
  * 20 SC_NS */
  sc_clock clk("clock", 10, SC_NS);

  // generate modules
  #if MASTER_N>0
  cout << "Generating master with index 1 ..." << endl;
  tlm2_simple_master TLM2_M1("TLM2_master0");
  cout << "Generating bus connector ..." << endl;
  tlm2_master_to_ahb AHB_M1("AHB_master0", 1);
  #else
  cout << "ERROR: cannot execute the AMBA AHB Bus without a master!" << endl;
  #endif

  #if MASTER_N>1
  cout << "Generating master with index 2 ..." << endl;
  tlm2_simple_master TLM2_M2("TLM2_master1");
  tlm2_master_to_ahb AHB_M2("AHB_master1", 2);
  #endif

  #if MASTER_N>2
  cout << "Generating master with index 3 ..." << endl;
  tlm2_simple_master TLM2_M3("TLM2_master2");
  tlm2_master_to_ahb AHB_M3("AHB_master2", 3);
  #endif

  #if AHB_SLAVE>0
  ahb_simple_mem AHB_S1("AHB_slave0", 0, MEM_SIZE);
  #endif

  #if AHB_SLAVE>1
  ahb_simple_mem AHB_S2("AHB_slave1", MEM_SIZE, MEM_SIZE);
  #endif


  #if TLM_SLAVE>0
  ahb_to_tlm2_slave AHB_TLM2_S1("AHB_to_TLM2_slave0", AHB_SLAVE * MEM_SIZE, MEM_SIZE);
  tlm2_simple_slave TLM2_S1("TLM2_slave0",AHB_SLAVE * MEM_SIZE, MEM_SIZE);
  #endif
  #if TLM_SLAVE>1
  ahb_to_tlm2_slave AHB_TLM2_S2("AHB_to_TLM2_slave1", (AHB_SLAVE + 1) * MEM_SIZE, MEM_SIZE);
  tlm2_simple_slave TLM2_S2("TLM2_slave1", (AHB_SLAVE + 1) * MEM_SIZE, MEM_SIZE);
  #endif

  ahb_simple_bus AHB_Bus("AHB_Bus");

  #if MASTER_N>0
    // connect clocks
  TLM2_M1.clk(clk);
  AHB_M1.clk(clk);
  #endif

  #if MASTER_N>1
  // connect clocks
  TLM2_M2.clk(clk);
  AHB_M2.clk(clk);
  #endif

  #if MASTER_N>2
  // connect clocks
  TLM2_M3.clk(clk);
  AHB_M3.clk(clk);
  #endif

  #if AHB_SLAVE>0
  AHB_S1.clk(clk);
  #endif

  #if AHB_SLAVE>1
  AHB_S2.clk(clk);
  #endif

  #if TLM_SLAVE>0
  AHB_TLM2_S1.clk(clk);
  TLM2_S1.clk(clk);
  #endif

  #if TLM_SLAVE>1
  AHB_TLM2_S2.clk(clk);
  TLM2_S2.clk(clk);
  #endif

  // connect modules
  #if MASTER_N>0
  TLM2_M1.init_sock(AHB_M1.slave_socket);
  AHB_M1.master_socket(AHB_Bus.slave_sock0);
  #endif

  #if MASTER_N>1
  TLM2_M2.init_sock(AHB_M2.slave_socket);
  AHB_M2.master_socket(AHB_Bus.slave_sock1);
  #endif

  #if MASTER_N>2
  TLM2_M3.init_sock(AHB_M3.slave_socket);
  AHB_M3.master_socket(AHB_Bus.slave_sock2);
  #endif

  #if AHB_SLAVE>0
  AHB_Bus.master_sock_ahb0(AHB_S1.slave_socket);
  #endif

  #if AHB_SLAVE>1
  AHB_Bus.master_sock_ahb1(AHB_S2.slave_socket);
  #endif

  #if TLM_SLAVE>0
  AHB_Bus.master_sock_tlm0(AHB_TLM2_S1.slave_socket);
  AHB_TLM2_S1.master_socket(TLM2_S1.slave_socket);
  #endif

  #if TLM_SLAVE>1
  AHB_Bus.master_sock_tlm1(AHB_TLM2_S2.slave_socket);
  AHB_TLM2_S2.master_socket(TLM2_S2.slave_socket);
  #endif

  cout << "Starting!" << endl;
  sc_start(100, SC_NS);

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
