//========================================================================================
//
// File Name    : adpcm_encoder.cpp
// Description  : Main ADPCM encoder description
// Release Date : 31/07/2013
//
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version         Author          Description
//----------------------------------------------------------------------------------------
// 2001-2003        1.0         ffmeg project   ADPCM  original ANSI Cdescription
// 31/07/2013        1.1         PolyU           Converted to SystemC (encoder only)
//
//=======================================================================================

/*
 * ADPCM codecs
 * Copyright (c) 2001-2003 The ffmpeg Project
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#include "adpcm_encoder.h"

void adpcm::run() {
    // LCOV_EXCL_BR_START
    struct width_data {
       sc_uint<16> in_data;    // Current input data
       sc_uint<16> pre_data;    // Previous input data
       sc_uint<18> diff_data;
       sc_uint<4> enc_data;
    } width;

    sc_uint<15> step_table[89] = {
      7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
      19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
      50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
      130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
      337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
      876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
      2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
      5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
      15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
    };

    sc_uint<7> index;
    sc_uint<4> index_delta;
    sc_uint<15> divider;
    sc_uint<15> remainder;
    sc_uint<2> remainder1;
    bool neg_flag;
    sc_uint<19> dec_tmp;
    sc_uint<32> temp = 0;
    sc_uint<17> diff;
    bool diff_tmp;

    wait();
    // LCOV_EXCL_BR_STOP

    // Main ADPCM Computation Loop
    while(true){
      width.in_data = idata.read();// LCOV_EXCL_BR_LINE
      divider = step_table[index];
      //cout << "width.in_data: " << width.in_data << "\n";

      // Encode
      diff = (width.in_data - width.pre_data) & 0x0000ffff;
      diff_tmp = diff[15]; // LCOV_EXCL_BR_LINE
      if( diff_tmp==1 ){
         width.diff_data = ((diff^0xffff) + 1);
         neg_flag = true;
      } else {
         width.diff_data = diff;
         neg_flag = false;
      }

      // LCOV_EXCL_BR_START
      width.diff_data = (width.diff_data<<2);
      temp = div_mod( width.diff_data, divider );
      width.enc_data = temp.range(31,17);
      remainder = temp.range(14,0);
      remainder *=2;
      // LCOV_EXCL_BR_STOP
      if( remainder >= divider )
        width.enc_data += 1;

      // Decode in the case of overflow
      if( width.enc_data > 7 ){
         width.enc_data = 7;
         dec_tmp = width.enc_data * divider;
         remainder1 = dec_tmp.range(1,0);// LCOV_EXCL_BR_LINE
         if( remainder1 >= 2 ){
            width.pre_data += (dec_tmp >> 2) + 1;
         } else {
            width.pre_data += (dec_tmp >> 2);
         }
      } else {
         width.pre_data = width.in_data;
      }

      // Output encoded data
      if( neg_flag == 1 ){
         odata.write(width.enc_data + 0x8); // LCOV_EXCL_BR_LINE
      } else {
         odata.write(width.enc_data); // LCOV_EXCL_BR_LINE
      }

      // Next step preparation
      index_delta = get_index_delta( width.enc_data );// LCOV_EXCL_BR_LINE

      if( index==0 && index_delta==1 ){
         index = 0;
      } else if( index_delta==1 ) {
         index -= 1;
      } else {
         index += index_delta;
      }

      wait(); // LCOV_EXCL_BR_LINE

    } // end while
}


sc_uint<4> adpcm::get_index_delta( sc_uint<4> enc )
{
   if( enc>=0 && enc<=3 )
     return 1;
   else if( enc==4 )
     return 2;
   else if( enc==5 )
     return 4;
   else if( enc==6 )
     return 6;
   else
     return 8;
}


sc_uint<32> adpcm::div_mod( sc_uint<18> numerator, sc_uint<15> denominator )
{
   // LCOV_EXCL_BR_START
   sc_uint<18> quotient=0;
   sc_uint<15> remainder;
   sc_uint<32> temp;
   sc_uint<18> d=0;
   int i;
   // LCOV_EXCL_BR_STOP
   for ( i=17; i>=0; i-- ) {
     d = (d<<1) + numerator[i];// LCOV_EXCL_BR_LINE

     if( d >= denominator ){
       d -= denominator;
       quotient += (0x1<<i);
     }
   }
   remainder = d;
   temp = (quotient<<17) + remainder;
   return temp;
} // LCOV_EXCL_BR_LINE
