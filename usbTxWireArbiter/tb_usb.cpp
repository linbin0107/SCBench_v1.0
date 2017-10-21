#include "tb_usb.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void testt::stimuli()
{
  rst.write(true);

  SIETxData.write(1);
  prcTxByteData.write(2);

  SIETxCtrl.write(false);
  SIETxFSRate.write(false);
  SIETxReq.write(false);
  SIETxWEn.write(false);

  prcTxByteCtrl.write(false);
  prcTxByteFSRate.write(false);
  prcTxByteReq.write(false);
  prcTxByteWEn.write(false);

  wait();
  rst.write(false);

  wait();
  SIETxCtrl.write(true);
  SIETxFSRate.write(true);
  SIETxReq.write(true);
  SIETxWEn.write(true);

  wait();
  prcTxByteCtrl.write(true);
  prcTxByteFSRate.write(true);
  prcTxByteReq.write(true);
  prcTxByteWEn.write(true);

  wait();
  SIETxCtrl.write(false);
  SIETxFSRate.write(false);
  SIETxReq.write(false);
  SIETxWEn.write(false);

  wait();
  SIETxCtrl.write(false);
  SIETxFSRate.write(true);
  SIETxReq.write(false);
  SIETxWEn.write(true);

  wait();
  prcTxByteCtrl.write(true);
  prcTxByteFSRate.write(false);
  prcTxByteReq.write(true);
  prcTxByteWEn.write(false);

  wait(2);
  compare_results();
  sc_stop();
}

void monitor::MonGen()
{
  ofstream ofs;
  ofs.open("usb_output.txt", ios::out | ios::app);

  ofs << sc_time_stamp() << " " << SIETxCtrl << " " <<
      SIETxData << " " << SIETxFSRate << " " << SIETxReq <<
      " " << SIETxWEn << " " << USBWireRdyIn << " " << clk <<
      " " << prcTxByteCtrl << " " << prcTxByteData << " " <<
      prcTxByteFSRate << " " << prcTxByteReq << " " <<
      prcTxByteWEn << " " << rst << "  $:" << SIETxGnt << " " <<
      TxBits << " " << TxCtl << " " << TxFSRate << " " <<
      USBWireRdyOut << " " << USBWireWEn << " " << prcTxByteGnt << endl;

  ofs.close();

}

void testt::compare_results()
{
  ifstream golden("usb_output_golden.txt");
  ifstream output("usb_output.txt");

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
