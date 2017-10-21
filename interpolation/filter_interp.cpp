
//========================================================================================
//
// File Name    : filter_interp.cpp
// Description  : Main interpolation filter function
// Release Date : 23/07/2013
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Anushree Mahapatra
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version   Author      Description
//----------------------------------------------------------------------------------------
//23/07/2013     1.0      PolyU      Interpolation filter description
//=======================================================================================
#include "filter_interp.h"

void interp::run( void )
{
  unsigned int n;

  //LCOV_EXCL_BR_START
  const sc_fixed<17,2,SC_TRN,SC_WRAP> coeff1[TAPS] =
  {
      0.002561105374,
      -0.0374560342,
      0.261339038123,
      0.236616189234,
      -0.0175371872,
      0.003662109373,
      0.040705008125,
      -0.0063781875
  };

  const sc_fixed<17,2,SC_TRN,SC_WRAP> coeff2[TAPS] =
  {
      -0.0012220704128,
      -0.00992526286873,
      0.28326416015625,
      -0.38756410156234,
      0.01092529296875,
      0.0111770703125,
      -0.01092529296875,
      0.28326416015625
  };

  const sc_fixed<17,2,SC_TRN,SC_WRAP> coeff3[TAPS] =
  {
      -0.0254359140667,
      0.12369433597894,
      -0.08766552334675,
      -0.08944552734377,
      0.4443443359323,
      -0.0172119140625,
      0.1116943359375,
      0.2222743352321
  };

  const sc_fixed<17,2,SC_TRN,SC_WRAP> coeff4[TAPS] =
  {
      -0.022432453112228,
      0.056155029296845,
      -0.562341259765625,
      0.34291256765612,
      -0.078155029296811,
      0.003434129453125,
      0.045560371453428,
      0.00014556453135
  };

  odata_en = 0;
  odata = 0;

  wait();
  //LCOV_EXCL_BR_STOP

  while (1)
  {
    //LCOV_EXCL_BR_START
    indata_read = indata.read();
    infactor_read = infactor.read();

    odata_en.write(0);
    //LCOV_EXCL_BR_STOP

    // Read inputs by shifting previous data
    for ( n = 5; n>0; n--)
        buffer[n] = buffer[n-1];//LCOV_EXCL_BR_LINE

    buffer[0] = indata_read;//LCOV_EXCL_BR_LINE

    // FIR 1 : Sum of Products of 1st filter
    SoP1 = 0;//LCOV_EXCL_BR_LINE
    for (n = 0; n < TAPS; n++ )
      SoP1 = SoP1 + buffer[n] * coeff1[n];//LCOV_EXCL_BR_LINE

    // FIR 2 : Sum of Products of 2nd filter
    SoP2 = 0;//LCOV_EXCL_BR_LINE
    for (n = 0; n < TAPS; n++ )
      SoP2 =SoP2 + buffer[n] * coeff2[n];//LCOV_EXCL_BR_LINE

    // FIR 3 : Sum of Products of 3rd filter
    SoP3 = 0;//LCOV_EXCL_BR_LINE
    for ( n = 0; n < TAPS; n++ )
      SoP3 = SoP3 + buffer[n] * coeff3[n];//LCOV_EXCL_BR_LINE

    // FIR 4 : Sum of Products of 4th filter
    SoP4 = 0;//LCOV_EXCL_BR_LINE
    for (n = 0; n < TAPS; n++ )
       SoP4 = SoP4 + buffer[n] * coeff4[n];//LCOV_EXCL_BR_LINE

    //LCOV_EXCL_BR_START
    // Output computation
    odata_write = SoP1 + SoP2 * infactor_read +
               SoP3 * infactor_read*infactor_read +
               SoP4 * infactor_read*infactor*infactor_read;

    // Manual polynomial decomposition (reduces the number of arithmetic operations -> leads to smaller design)
    //tmp1 = SoP4*infactor_read + SoP3;
    //tmp2 = tmp1 * infactor_read + SoP2;
    //odata_write = tmp2 * infactor_read + SoP1;

    // Write results back
    odata_en.write( 1 );
    odata.write( odata_write );

    wait();
  }
}
//LCOV_EXCL_BR_STOP
