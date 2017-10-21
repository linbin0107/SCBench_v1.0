#include <iostream>
#include <stdint.h>
#include "usbTxWireArbiter.h"

//--------------------------------
// processTxByte/SIETransmitter mux
//--------------------------------
void USBTxWireArbiter::txWireArb_mux()
{
  USBWireRdyOut.write(USBWireRdyIn.read());//LCOV_EXCL_BR_LINE
}

void USBTxWireArbiter::txWireArb_arb()
{
  bool mux = muxSIENotPTXB.read();//LCOV_EXCL_BR_LINE
  if (mux == true)
  {
    //LCOV_EXCL_BR_START
    USBWireWEn.write(SIETxWEn.read());
    TxBits.write(SIETxData.read());
    TxCtl.write(SIETxCtrl.read());
    TxFSRate.write(SIETxFSRate.read());
    //LCOV_EXCL_BR_STOP
  }
  else
  {
    //LCOV_EXCL_BR_START
    USBWireWEn.write(prcTxByteWEn.read());
    TxBits.write(prcTxByteData.read());
    TxCtl.write(prcTxByteCtrl.read());
    TxFSRate.write(prcTxByteFSRate.read());
    //LCOV_EXCL_BR_STOP
  }
}

//--------------------------------------------------------------------
// Machine: txWireArb
//--------------------------------------------------------------------
//----------------------------------
// Next State Logic (combinatorial)
//----------------------------------
void USBTxWireArbiter::txWireArb_NextState()
{
  uint8_t cs_tx = 0;
  bool prcTxByteReq_tmp;
  bool SIETxReq_tmp;
  //LCOV_EXCL_BR_START
  NextState_txWireArb.write(CurrState_txWireArb.read());
  // Set default values for outputs and signals
  next_prcTxByteGnt.write(prcTxByteGnt.read());
  next_muxSIENotPTXB.write(muxSIENotPTXB.read());
  next_SIETxGnt.write(SIETxGnt.read());
  //LCOV_EXCL_BR_STOP

  cs_tx = CurrState_txWireArb.read();//LCOV_EXCL_BR_LINE
  switch(cs_tx)
  {
    case START_TARB:
      NextState_txWireArb.write(TARB_WAIT_REQ);//LCOV_EXCL_BR_LINE
      break;
    case TARB_WAIT_REQ:
      //LCOV_EXCL_BR_START
      prcTxByteReq_tmp = prcTxByteReq.read();
      SIETxReq_tmp = SIETxReq.read();
      //LCOV_EXCL_BR_STOP
      if (prcTxByteReq_tmp == true)
      {
        //LCOV_EXCL_BR_START
        NextState_txWireArb.write(PTXB_ACT);
        next_prcTxByteGnt.write(true);
        next_muxSIENotPTXB.write(false);
        //LCOV_EXCL_BR_STOP
      }
      else if (SIETxReq_tmp == true)
      {
        //LCOV_EXCL_BR_START
        NextState_txWireArb.write(SIE_TX_ACT);
        next_SIETxGnt.write(true);
        next_muxSIENotPTXB.write(true);
        //LCOV_EXCL_BR_STOP
      }
      break;
    case PTXB_ACT:
      prcTxByteReq_tmp = prcTxByteReq.read();//LCOV_EXCL_BR_LINE
      if (prcTxByteReq_tmp == false)
      {
        //LCOV_EXCL_BR_START
        NextState_txWireArb.write(TARB_WAIT_REQ);
        next_prcTxByteGnt.write(false);
        //LCOV_EXCL_BR_STOP
      }
      break;
    case SIE_TX_ACT:
      SIETxReq_tmp = SIETxReq.read();//LCOV_EXCL_BR_LINE
      if (SIETxReq_tmp == false)
      {
       //LCOV_EXCL_BR_START
        NextState_txWireArb.write(TARB_WAIT_REQ);
        next_SIETxGnt.write(false);
        //LCOV_EXCL_BR_STOP
      }
      break;
  }
}

//----------------------------------
// Current State Logic (sequential)
//----------------------------------
void USBTxWireArbiter::txWireArb_CurrentState()
{
  bool rst_tmp = rst.read();//LCOV_EXCL_BR_LINE
  if (rst_tmp)
    CurrState_txWireArb.write(START_TARB);//LCOV_EXCL_BR_LINE
  else
    CurrState_txWireArb.write(NextState_txWireArb.read());//LCOV_EXCL_BR_LINE
}

//----------------------------------
// Registered outputs logic
//----------------------------------
void USBTxWireArbiter::txWireArb_RegOutput()
{
  bool rst_tmp = rst.read();//LCOV_EXCL_BR_LINE
  if (rst_tmp)
  {
    //LCOV_EXCL_BR_START
    muxSIENotPTXB.write(false);
    prcTxByteGnt.write(false);
    SIETxGnt.write(false);
    //LCOV_EXCL_BR_STOP
  }
  else
  {
    //LCOV_EXCL_BR_START
    muxSIENotPTXB.write(next_muxSIENotPTXB.read());
    prcTxByteGnt.write(next_prcTxByteGnt.read());
    SIETxGnt.write(next_SIETxGnt.read());
    //LCOV_EXCL_BR_STOP
  }
}//LCOV_EXCL_BR_LINE
