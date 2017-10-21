#include "systemc.h"

// BINARY ENCODED state machine: txWireArb
// State codes definitions:
#define START_TARB 0
#define TARB_WAIT_REQ 1
#define PTXB_ACT 2
#define SIE_TX_ACT 3

//LCOV_EXCL_BR_START
SC_MODULE(USBTxWireArbiter)
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

  sc_out<bool>	SIETxGnt;
  sc_out<sc_uint<2> >	TxBits;
  sc_out<bool>	TxCtl;
  sc_out<bool>	TxFSRate;
  sc_out<bool>	USBWireRdyOut;
  sc_out<bool>	USBWireWEn;
  sc_out<bool>	prcTxByteGnt;

  void txWireArb_mux();
  void txWireArb_arb();
  void txWireArb_NextState();
  void txWireArb_CurrentState();
  void txWireArb_RegOutput();

  sc_signal<bool>  next_SIETxGnt;
  sc_signal<sc_uint<2> >	next_TxBits;
  sc_signal<bool>  	next_TxCtl;
  sc_signal<bool>	next_TxFSRate;
  sc_signal<bool>	next_USBWireRdyOut;
  sc_signal<bool>	next_USBWireWEn;
  sc_signal<bool>	next_prcTxByteGnt;

  sc_signal<bool>	muxSIENotPTXB;
  sc_signal<bool>	next_muxSIENotPTXB;

  sc_signal<sc_uint<2> >	CurrState_txWireArb;
  sc_signal<sc_uint<2> >	NextState_txWireArb;

  SC_CTOR(USBTxWireArbiter)
  {
    SC_METHOD(txWireArb_mux);
    sensitive << USBWireRdyIn;

    SC_METHOD(txWireArb_arb);
    sensitive << muxSIENotPTXB << SIETxWEn << SIETxData << SIETxCtrl << prcTxByteWEn << prcTxByteData << prcTxByteCtrl <<
SIETxFSRate << prcTxByteFSRate;

    SC_METHOD(txWireArb_NextState);
    sensitive << prcTxByteReq << SIETxReq << prcTxByteGnt << muxSIENotPTXB << SIETxGnt << CurrState_txWireArb;

    SC_METHOD(txWireArb_CurrentState);
    sensitive << clk.pos();

    SC_METHOD(txWireArb_RegOutput);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP
