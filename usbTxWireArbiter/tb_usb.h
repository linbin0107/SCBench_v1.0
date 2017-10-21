#include "systemc.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
SC_MODULE(testt)
{
  sc_in<bool>	clk;
  sc_out<bool>	SIETxCtrl;
  sc_out<sc_uint<2> >	SIETxData;
  sc_out<bool>	SIETxFSRate;
  sc_out<bool>	SIETxReq;
  sc_out<bool>	SIETxWEn;

  sc_out<bool>	USBWireRdyIn;

  sc_out<bool>	prcTxByteCtrl;
  sc_out<sc_uint<2> >	prcTxByteData;
  sc_out<bool>	prcTxByteFSRate;
  sc_out<bool>	prcTxByteReq;
  sc_out<bool>	prcTxByteWEn;

  sc_out<bool>	rst;

  void compare_results();
  void stimuli();

  SC_CTOR(testt)
  {
    SC_CTHREAD(stimuli, clk.pos());
  }
};

SC_MODULE(monitor)
{
  sc_in<bool>	SIETxCtrl;
  sc_in<sc_uint<2> >	SIETxData;
  sc_in<bool>	SIETxFSRate;
  sc_in<bool>	SIETxReq;
  sc_in<bool>	SIETxWEn;
  sc_in<bool>	USBWireRdyIn;
  sc_in<bool>	clk;
  sc_in<bool>	prcTxByteCtrl;
  sc_in<sc_uint<2> >	prcTxByteData;
  sc_in<bool>	prcTxByteFSRate;
  sc_in<bool>	prcTxByteReq;
  sc_in<bool>	prcTxByteWEn;
  sc_in<bool>	rst;

  sc_in<bool>	SIETxGnt;
  sc_in<sc_uint<2> >	TxBits;
  sc_in<bool>	TxCtl;
  sc_in<bool>	TxFSRate;
  sc_in<bool>	USBWireRdyOut;
  sc_in<bool>	USBWireWEn;
  sc_in<bool>	prcTxByteGnt;

  void MonGen();

  SC_CTOR(monitor)
  {
    SC_METHOD(MonGen);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
