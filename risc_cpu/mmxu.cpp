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

/*****************************************************************************

  mmxu.cpp -- MMX-Like Execution Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Nicolas Blanc, ETHZ, 2009
  Description of Modification: many code updates

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "mmxu.h"

void mmxu::entry(){
  //LCOV_EXCL_BR_START
  sc_uint<8>		        opcode_tmp = 0;
  sc_uint<32>		        dout_tmp = 0;

  sc_uint<32>			dina_tmp = 0;
  sc_uint<32>			dinb_tmp = 0;

  sc_uint<32>			dina0_tmp = 0;
  sc_uint<32>			dina1_tmp = 0;
  sc_uint<32>			dina2_tmp = 0;
  sc_uint<32>			dina3_tmp = 0;
  sc_uint<32>			dinb0_tmp = 0;
  sc_uint<32>			dinb1_tmp = 0;
  sc_uint<32>			dinb2_tmp = 0;
  sc_uint<32>			dinb3_tmp = 0;
  sc_uint<32>			mmxc0_tmp = 0;
  sc_uint<32>			mmxc1_tmp = 0;
  sc_uint<32>			mmxc2_tmp = 0;
  sc_uint<32>			mmxc3_tmp = 0;
  sc_uint<32>			mmxcU_tmp = 0;
  sc_uint<32>			mmxcL_tmp = 0;
  //LCOV_EXCL_BR_STOP

  bool cs_tmp = false;

  // main loop
  // initialization of output
  wait(3);
  while(1) {
    cs_tmp = cs.read(); // LCOV_EXCL_BR_LINE
   	if (cs_tmp == true) {
  	  dina_tmp = dina.read(); //LCOV_EXCL_BR_LINE
  	  dinb_tmp = dinb.read(); //LCOV_EXCL_BR_LINE

      dina0_tmp = (dina_tmp & 0x000000ff) ;
      dinb0_tmp = (dinb_tmp & 0x000000ff) ;
      dina1_tmp = (dina_tmp & 0x0000ff00) >> 8 ;
      dinb1_tmp = (dinb_tmp & 0x0000ff00) >> 8 ;
      dina2_tmp = (dina_tmp & 0x00ff0000) >> 16 ;
      dinb2_tmp = (dinb_tmp & 0x00ff0000) >> 16 ;
      dina3_tmp = (dina_tmp & 0xff000000) >> 24 ;
      dinb3_tmp = (dinb_tmp & 0xff000000) >> 24 ;
  	  opcode_tmp = opcode.read();//LCOV_EXCL_BR_LINE

      // output MUX
      switch (opcode_tmp) {
        case 0: // Stall
       	  dout_tmp = dout_tmp;
       	  wait();//LCOV_EXCL_BR_LINE
       	  break;

        case 3: // add
          mmxc3_tmp = dina3_tmp + dinb3_tmp;
          mmxc2_tmp = dina2_tmp + dinb2_tmp;
          mmxc1_tmp = dina1_tmp + dinb1_tmp;
          mmxc0_tmp = dina0_tmp + dinb0_tmp;
          mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
          mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
          mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
          dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
       	  wait(); //LCOV_EXCL_BR_LINE
       	  break;

        case 4: // add with saturation
          mmxc3_tmp = dina3_tmp + dinb3_tmp;
          mmxc2_tmp = dina2_tmp + dinb2_tmp;
          mmxc1_tmp = dina1_tmp + dinb1_tmp;
          mmxc0_tmp = dina0_tmp + dinb0_tmp;

          if (mmxc3_tmp >= 256)
            mmxc3_tmp = 0xff;
          if (mmxc2_tmp >= 256)
            mmxc2_tmp = 0xff;
          if (mmxc1_tmp >= 256)
            mmxc1_tmp = 0xff;
          if (mmxc0_tmp >= 256)
            mmxc0_tmp = 0xff;

          mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
          mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
          mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
          dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
          wait(); //LCOV_EXCL_BR_LINE
          break;

        case 5: // sub
          mmxc3_tmp = dina3_tmp - dinb3_tmp;
          mmxc2_tmp = dina2_tmp - dinb2_tmp;
          mmxc1_tmp = dina1_tmp - dinb1_tmp;
          mmxc0_tmp = dina0_tmp - dinb0_tmp;
          mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
          mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
          mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
          dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
          wait();//LCOV_EXCL_BR_LINE
          break;

        case 6: // sub with saturation
          mmxc3_tmp = dina3_tmp - dinb3_tmp;
          mmxc2_tmp = dina2_tmp - dinb2_tmp;
          mmxc1_tmp = dina1_tmp - dinb1_tmp;
          mmxc0_tmp = dina0_tmp - dinb0_tmp;

          if (dinb3_tmp > dina3_tmp)
            mmxc3_tmp = 0x00;
          if (dinb2_tmp > dina2_tmp)
            mmxc2_tmp = 0x00;
          if (dinb1_tmp > dina1_tmp)
            mmxc1_tmp = 0x00;
          if (dinb0_tmp > dina0_tmp)
            mmxc0_tmp = 0x00;

          mmxc3_tmp = (mmxc3_tmp << 24) & 0xff000000;
          mmxc2_tmp = (mmxc2_tmp << 16) & 0x00ff0000;
          mmxc1_tmp = (mmxc1_tmp << 8)  & 0x0000ff00;
          dout_tmp  = mmxc0_tmp | mmxc1_tmp | mmxc2_tmp | mmxc3_tmp;
          wait(); //LCOV_EXCL_BR_LINE
          break;

        case 7:// packed multiply add with saturation
   	      // a3*b3+a2*b2 , a1*b1+a0*b0
          mmxc3_tmp = dina3_tmp * dinb3_tmp;
          mmxc2_tmp = dina2_tmp * dinb2_tmp;
          mmxc1_tmp = dina1_tmp * dinb1_tmp;
          mmxc0_tmp = dina0_tmp * dinb0_tmp;

          if (mmxc3_tmp >= 256)
            mmxc3_tmp = 0xff;
          if (mmxc2_tmp >= 256)
            mmxc2_tmp = 0xff;
          if (mmxc1_tmp >= 256)
            mmxc1_tmp = 0xff;
          if (mmxc0_tmp >= 256)
            mmxc0_tmp = 0xff;

          mmxcU_tmp = mmxc3_tmp + mmxc2_tmp;
          mmxcL_tmp = mmxc1_tmp + mmxc0_tmp;

          if (mmxcU_tmp >= 256)
            mmxcU_tmp = 0xff;
          if (mmxcL_tmp >= 256)
            mmxcL_tmp = 0xff;

          mmxcU_tmp = (mmxcU_tmp << 16) ;
          dout_tmp  = mmxcU_tmp | mmxcL_tmp;
          wait();//LCOV_EXCL_BR_LINE
          break;

        case 8:// packed b1 (16bit) b0(16bit) and a1(16 bit) a0(16 bit)
          // to B1(8bit) B0(8bit) A1(8bit) A0(8bit)
          mmxc3_tmp = dinb2_tmp << 24;
          mmxc2_tmp = dinb0_tmp << 16;
          mmxc1_tmp = dina2_tmp << 8;
          mmxc0_tmp = dina0_tmp ;
          dout_tmp  = mmxc3_tmp | mmxc2_tmp | mmxc1_tmp | mmxc0_tmp;
          wait();//LCOV_EXCL_BR_LINE
          break;

        case 9: // mmx chroma keying
          // A =green != green green !=green
   	      // B =green    green green   green
          // Res=0xff  00   ff    00
          if (dina3_tmp == dinb3_tmp)
   	        mmxc3_tmp = 0xff;
          else
   	        mmxc3_tmp = 0x00;

          if (dina2_tmp == dinb2_tmp)
   	        mmxc2_tmp = 0xff;
          else
   	        mmxc2_tmp = 0x00;

          if (dina1_tmp == dinb1_tmp)
   	        mmxc1_tmp = 0xff;
          else
   	        mmxc1_tmp = 0x00;

          if (dina0_tmp == dinb0_tmp)
   	        mmxc0_tmp = 0xff;
          else
   	        mmxc0_tmp = 0x00;

          mmxc3_tmp = mmxc3_tmp << 24;
          mmxc2_tmp = mmxc2_tmp << 16;
          mmxc1_tmp = mmxc1_tmp << 8;
          mmxc0_tmp = mmxc0_tmp ;
          dout_tmp  = mmxc3_tmp | mmxc2_tmp | mmxc1_tmp | mmxc0_tmp;
          wait();//LCOV_EXCL_BR_LINE
          break;

        default:
          cout <<"MMX:      Bad Opcode "<< opcode_tmp << "\n";//LCOV_EXCL_BR_LINE
			wait();//LCOV_EXCL_BR_LINE
			break;
      }

      // LCOV_EXCL_BR_START
      dout.write(sc_int<32>(dout_tmp));
      out_valid.write(true);

      wait();
      out_valid.write(false);
      wait();
      // LCOV_EXCL_BR_STOP
    } else {
		wait(); // LCOV_EXCL_BR_LINE
    }

  }

}//LCOV_EXCL_BR_LINE

