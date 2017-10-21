#include "systemc.h"
#include <iostream>
#include "usbTxWireArbiter.h"
#include "tb_usb.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[])
{
  sc_signal<bool>	SIETxCtrl;
  sc_signal<sc_uint<2> >	SIETxData;
  sc_signal<bool>	SIETxFSRate;
  sc_signal<bool>	SIETxReq;
  sc_signal<bool>	SIETxWEn;
  sc_signal<bool>	USBWireRdyIn;
  sc_signal<bool>	prcTxByteCtrl;
  sc_signal<sc_uint<2> >	prcTxByteData;
  sc_signal<bool>	prcTxByteFSRate;
  sc_signal<bool>	prcTxByteReq;
  sc_signal<bool>	prcTxByteWEn;
  sc_signal<bool>	rst;

  sc_signal<bool>	SIETxGnt;
  sc_signal<sc_uint<2> >	TxBits;
  sc_signal<bool>	TxCtl;
  sc_signal<bool>	TxFSRate;
  sc_signal<bool>	USBWireRdyOut;
  sc_signal<bool>	USBWireWEn;
  sc_signal<bool>	prcTxByteGnt;

  sc_clock clk("clk", 10, SC_NS);

  USBTxWireArbiter ins("ins");
  ins.SIETxCtrl(SIETxCtrl);
  ins.SIETxData(SIETxData);
  ins.SIETxFSRate(SIETxFSRate);
  ins.SIETxReq(SIETxReq);
  ins.SIETxWEn(SIETxWEn);
  ins.USBWireRdyIn(USBWireRdyIn);
  ins.clk(clk);
  ins.prcTxByteCtrl(prcTxByteCtrl);
  ins.prcTxByteData(prcTxByteData);
  ins.prcTxByteFSRate(prcTxByteFSRate);
  ins.prcTxByteReq(prcTxByteReq);
  ins.prcTxByteWEn(prcTxByteWEn);
  ins.rst(rst);
  ins.SIETxGnt(SIETxGnt);
  ins.TxBits(TxBits);
  ins.TxCtl(TxCtl);
  ins.TxFSRate(TxFSRate);
  ins.USBWireRdyOut(USBWireRdyOut);
  ins.USBWireWEn(USBWireWEn);
  ins.prcTxByteGnt(prcTxByteGnt);

  testt tb("tb");
  tb.SIETxCtrl(SIETxCtrl);
  tb.SIETxData(SIETxData);
  tb.SIETxFSRate(SIETxFSRate);
  tb.SIETxReq(SIETxReq);
  tb.SIETxWEn(SIETxWEn);
  tb.USBWireRdyIn(USBWireRdyIn);
  tb.clk(clk);
  tb.prcTxByteCtrl(prcTxByteCtrl);
  tb.prcTxByteData(prcTxByteData);
  tb.prcTxByteFSRate(prcTxByteFSRate);
  tb.prcTxByteReq(prcTxByteReq);
  tb.prcTxByteWEn(prcTxByteWEn);
  tb.rst(rst);

  monitor mon("mon");
  mon.SIETxCtrl(SIETxCtrl);
  mon.SIETxData(SIETxData);
  mon.SIETxFSRate(SIETxFSRate);
  mon.SIETxReq(SIETxReq);
  mon.SIETxWEn(SIETxWEn);
  mon.USBWireRdyIn(USBWireRdyIn);
  mon.clk(clk);
  mon.prcTxByteCtrl(prcTxByteCtrl);
  mon.prcTxByteData(prcTxByteData);
  mon.prcTxByteFSRate(prcTxByteFSRate);
  mon.prcTxByteReq(prcTxByteReq);
  mon.prcTxByteWEn(prcTxByteWEn);
  mon.rst(rst);
  mon.SIETxGnt(SIETxGnt);
  mon.TxBits(TxBits);
  mon.TxCtl(TxCtl);
  mon.TxFSRate(TxFSRate);
  mon.USBWireRdyOut(USBWireRdyOut);
  mon.USBWireWEn(USBWireWEn);
  mon.prcTxByteGnt(prcTxByteGnt);

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("USB");

  sc_trace(trace_file, SIETxCtrl, "SIETxCtrl");
  sc_trace(trace_file, SIETxData, "SIETxData");
  sc_trace(trace_file, SIETxFSRate, "SIETxFSRate");
  sc_trace(trace_file, SIETxReq, "SIETxReq");
  sc_trace(trace_file, SIETxWEn, "SIETxWEn");
  sc_trace(trace_file, USBWireRdyIn, "USBWireRdyIn");
  sc_trace(trace_file, prcTxByteCtrl, "prcTxByteCtrl");
  sc_trace(trace_file, prcTxByteData, "prcTxByteData");
  sc_trace(trace_file, prcTxByteFSRate, "prcTxByteFSRate");
  sc_trace(trace_file, prcTxByteReq, "prcTxByteReq");
  sc_trace(trace_file, prcTxByteWEn, "prcTxByteWEn");
  sc_trace(trace_file, rst, "rst");
  sc_trace(trace_file, SIETxGnt, "SIETxGnt");
  sc_trace(trace_file, TxBits, "TxBits");
  sc_trace(trace_file, TxCtl, "TxCtl");
  sc_trace(trace_file, TxFSRate, "TxFSRate");
  sc_trace(trace_file, USBWireRdyOut, "USBWireRdyOut");
  sc_trace(trace_file, USBWireWEn, "USBWireWEn");
  sc_trace(trace_file, prcTxByteGnt, "prcTxByteGnt");
  sc_trace(trace_file, clk, "clk");
#endif

  sc_start();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
