#include "systemc.h"

// constants

//LCOV_EXCL_BR_START
SC_MODULE(cpu_Y86)
{
  sc_in<bool>	clk_;
  sc_in<bool>	rst_;
  sc_out<sc_uint<32> > bus_A_;
  sc_in<sc_uint<32> > bus_in_;
  sc_out<sc_uint<32> > bus_out_;
  sc_out<bool>	bus_WE_;
  sc_out<bool>	bus_RE_;
  sc_out<sc_uint<8> >	current_opcode_;

  void stage_0_pre_assign();
  void stage_0();
  void process_IF();
  void process_ID_pre_assign();
  void process_ID();
  void process_EX_pre_assign();
  void process_EX();
  void process_MEM();
  void process_WB_pre_assign();
  void process_WB();
  void process_WB_aft_assign();

  // global control variables
  sc_uint<5> full;
  sc_signal<sc_uint<5> > ue;

  // stage 1 IF variable
  sc_uint<32> IR;

  // stage 2 ID variables
  sc_uint<32> IP;
  sc_uint<32> A;
  sc_uint<32> B;
  sc_signal<sc_uint<32> > Aop;
  sc_signal<sc_uint<32> > Bop;
  sc_signal<sc_uint<8> > opcode;
  sc_signal<sc_uint<2> > mod;
  bool ZF;
  sc_signal<bool> load;
  sc_signal<bool> move;
  sc_signal<bool> store;
  sc_signal<bool> memory;
  sc_signal<bool> add;
  sc_signal<bool> sub;
  sc_signal<bool> halt;
  sc_signal<bool> aluop;
  sc_signal<bool> jnez;
  sc_signal<sc_uint<5> > RD;
  sc_signal<sc_uint<5> > RS;
  sc_signal<sc_uint<5> > Aad;
  sc_signal<sc_uint<5> > Bad;
  sc_signal<sc_uint<32> > distance;
  sc_signal<sc_uint<32> > displacement;
  sc_signal<bool> btaken;
  sc_signal<sc_uint<2> > length;

  // stage 3 EX
  sc_uint<32> MAR;
  sc_uint<32> MDRw;
  sc_uint<32> C;
  sc_signal<sc_uint<32> > ALU_op2;
  sc_signal<sc_uint<32> > ALUout;

  // stage 4 MEM
  sc_uint<32> MDRr;

  // stage 5 WB
  sc_uint<32> R[8];

  sc_signal<sc_uint<32> > eax;
  sc_signal<sc_uint<32> > ecx;
  sc_signal<sc_uint<32> > edx;
  sc_signal<sc_uint<32> > ebx;
  sc_signal<sc_uint<32> > esp;
  sc_signal<sc_uint<32> > ebp;
  sc_signal<sc_uint<32> > esi;
  sc_signal<sc_uint<32> > edi;
  sc_signal<sc_uint<32> > regs;

  SC_CTOR(cpu_Y86)
  {
    SC_THREAD(stage_0_pre_assign);
    sensitive << clk_.pos();

    SC_THREAD(stage_0);
    sensitive << clk_.pos();

    SC_METHOD(process_IF);
    sensitive << clk_.pos();

    SC_THREAD(process_ID_pre_assign);
    sensitive << clk_.pos();

    SC_METHOD(process_ID);
    sensitive << clk_.pos() << rst_.pos();

    SC_THREAD(process_EX_pre_assign);
    sensitive << clk_.pos();

    SC_METHOD(process_EX);
    sensitive << clk_.pos() << rst_.pos();

    SC_METHOD(process_MEM);
    sensitive << clk_.pos();

    SC_THREAD(process_WB_pre_assign);
    sensitive << clk_.pos();

    SC_METHOD(process_WB);
    sensitive << clk_.pos() << rst_.pos();

    SC_THREAD(process_WB_aft_assign);
    sensitive << clk_.pos();
  }
};
//LCOV_EXCL_BR_STOP
