
#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_exec){
  sc_in< bool > clk;
  sc_in<sc_int<32> > result;
  sc_in<bool> carry;
  sc_in<bool> overflow;
  sc_in<bool> zero;
  sc_in<bool> isValid;
  sc_in<bool> isBusy;

  sc_out< bool > cs_out;
  sc_out<sc_uint<8> > opcode_out;
  sc_out<sc_int<32> > dina_out;
  sc_out<sc_int<32> > dinb_out;

  void compare_results();
  void driver();
  void monitor();

  SC_CTOR(tb_exec) {
    SC_CTHREAD(driver, clk.pos());
    SC_CTHREAD(monitor, clk.pos());
  }

  ~tb_exec() { }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
