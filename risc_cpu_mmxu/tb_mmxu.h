
#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(tb_mmxu){
  sc_in< bool > clk;
  sc_in<bool> isValid;
  sc_in<bool> isBusy;
  sc_in<sc_int<32> > result;

  sc_out< bool > cs_out; // MMX cs
  sc_out<sc_uint<8> > opcode_out; // opcode
  sc_out<sc_int<32> > dina_out; // operand A
  sc_out<sc_int<32> > dinb_out; // operand B

  void compare_results();
  void driver();
  void monitor();

  SC_CTOR(tb_mmxu) {
    SC_CTHREAD(driver, clk.pos());
    SC_CTHREAD(monitor, clk.pos());
  }

  ~tb_mmxu() { }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
