//---------------------------------------------------------------------------
//
// File Name    : fft.cpp
// Description  : FFT implementation
// Release Date : 16/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date     Version   Author          Description
//---------------------------------------------------------------------------
//            1.0   R.Goswami,Synopsys, Inc.  fft.cpp - This is the implementation file for the synchronous process "fft"
// 16/07/03   1.1   PolyU DARC Lab
//---------------------------------------------------------------------------

/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#include "fft.h"


void fft::entry()
{
  // Note: Float is not synthesizable
  float sample[16][2];
  unsigned int index;

  // Reset state -- should be executable in a single clock cycle
  for (unsigned int i = 0; i < 16; i++) {
    for(unsigned h=0; h <2; h++){
 	  sample[i][h] = 0;
	}
  }

  index = 0;

  wait();//LCOV_EXCL_BR_LINE

  // Main computational loop
  while(true)
  {
    //LCOV_EXCL_BR_START
    data_req.write(false);
    data_ready.write(false);
    index = 0;
    //LCOV_EXCL_BR_STOP

    //Reading in the Samples
    while( index < 16 )
    {
      //LCOV_EXCL_BR_START
      data_req.write(true);
      wait();
      bool data_valid_tmp = data_valid.read();
      //LCOV_EXCL_BR_STOP
      while (data_valid_tmp ==  true)
      {
        wait();//LCOV_EXCL_BR_LINE
        data_valid_tmp = data_valid.read();//LCOV_EXCL_BR_LINE
      }

      //LCOV_EXCL_BR_START
      sample[index][0] = in_real.read();
      sample[index][1] = in_imag.read();
      index++;

      data_req.write(false);
      wait();
      //LCOV_EXCL_BR_STOP
    }

    index = 0;

    //////////////////////////////////////////////////////////////////////////
    ///  Computation - 1D Complex DFT In-Place DIF Computation Algorithm  ////
    //////////////////////////////////////////////////////////////////////////

    //Size of FFT, N = 2**M
    unsigned int N, M, len ;
    float theta;
    float W[7][2], w_real, w_imag, w_rec_real, w_rec_imag, w_temp;

    //Initialize
    M = 4; N = 16;
    len = N/2;
    theta = 8*atan(1.0)/N;

    //  cout << "Computing..." << endl;

    //Calculate the W-values recursively
    w_real =  cos(theta);
    w_imag =  -sin(theta);

    w_rec_real = 1;
    w_rec_imag = 0;

    index = 0;

    while(index < len-1)
    {
       w_temp = w_rec_real*w_real - w_rec_imag*w_imag;
       w_rec_imag =  w_rec_real*w_imag + w_rec_imag*w_real;
       w_rec_real = w_temp;
       W[index][0] = w_rec_real;
       W[index][1] = w_rec_imag;
       index++;
    }

    float tmp_real, tmp_imag, tmp_real2, tmp_imag2;
    unsigned int stage, i, j,index2, windex, incr;

    //Begin Computation
    stage = 0;

    len = N;
    incr = 1;

    while (stage < M)
    {
      len = len/2;

      //First Iteration :  With No Multiplies
      i = 0;
      while(i < N)
      {
        index =  i; index2 = index + len;

        tmp_real = sample[index][0] + sample[index2][0];
        tmp_imag = sample[index][1] + sample[index2][1];

        sample[index2][0] = sample[index][0] - sample[index2][0];
        sample[index2][1] = sample[index][1] - sample[index2][1];

        sample[index][0] = tmp_real;
        sample[index][1] = tmp_imag;

        i = i + 2*len;
      }

      //Remaining Iterations: Use Stored W
      j = 1; windex = incr - 1;
      // This loop executes N/2 times at first stage, .. once at last stage.
      while (j < len)
      {
        i = j;
        while (i < N)
        {
          index = i;
          index2 = index + len;

          tmp_real = sample[index][0] + sample[index2][0];
          tmp_imag = sample[index][1] + sample[index2][1];
          tmp_real2 = sample[index][0] - sample[index2][0];
          tmp_imag2 = sample[index][1] - sample[index2][1];

          sample[index2][0] = tmp_real2*W[windex][0] - tmp_imag2*W[windex][1];
          sample[index2][1] = tmp_real2*W[windex][1] + tmp_imag2*W[windex][0];

          sample[index][0] = tmp_real;
          sample[index][1] = tmp_imag;

          i = i + 2*len;
        }

        windex = windex + incr;
        j++;
      }

       stage++;
       incr = 2*incr;
    }

    //Writing out the normalized transform values in bit reversed order
    //LCOV_EXCL_BR_START
    sc_uint<4> bits_i;
    sc_uint<4> bits_index;
    bits_i = 0;
    i = 0;
    //LCOV_EXCL_BR_STOP

    //cout << "Writing the transform values..." << endl;
    while( i < 16)
    {
      //LCOV_EXCL_BR_START
      bits_i = i;
      bits_index[3]= bits_i[0];
      bits_index[2]= bits_i[1];
      bits_index[1]= bits_i[2];
      bits_index[0]= bits_i[3];
      index = bits_index;

      out_real.write(sample[index][0]);
      out_imag.write(sample[index][1]);
      data_ready.write(true);

      wait();
      bool data_ack_tmp = data_ack.read();
      //LCOV_EXCL_BR_STOP

      while (data_ack_tmp != true)
      {
        wait();//LCOV_EXCL_BR_LINE
        data_ack_tmp = data_ack.read();//LCOV_EXCL_BR_LINE
      }

      data_ready.write(false);//LCOV_EXCL_BR_LINE
      i++;

      wait();//LCOV_EXCL_BR_LINE
    }

    index = 0;
    // cout << "Done..." << endl;
  }
}//LCOV_EXCL_BR_LINE
