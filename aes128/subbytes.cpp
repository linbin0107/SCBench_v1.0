//////////////////////////////////////////////////////////////////////
////                                                              ////
////  AES subbytes module implementation                          ////
////                                                              ////
////  This file is part of the SystemC AES                        ////
////                                                              ////
////  Description:                                                ////
////  Subbytes stage implementation for AES algorithm             ////
////                                                              ////
////  To Do:                                                      ////
////   - done                                                     ////
////                                                              ////
////  Author(s):                                                  ////
////      - Javier Castillo, jcastilo@opencores.org               ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2000 Authors and OPENCORES.ORG                 ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free software; you can redistribute it   ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: subbytes.cpp,v $
// Revision 1.1  2007-10-13 14:56:14  blancn
// *** empty log message ***
//
// Revision 1.3  2005/03/16 18:12:25  jcastillo
//
// Style modifications
//
// Revision 1.1  2005/01/26 16:51:06  jcastillo
// New examples for 0.2.5 version
//
// Revision 1.2  2004/08/30 14:44:44  jcastillo
// Code Formater used to give better appearance to SystemC code
//
// Revision 1.1.1.1  2004/07/05 09:46:22  jcastillo
// First import
//

#include "subbytes.h"

void subbytes::sub()
{
    //LCOV_EXCL_BR_START
	sc_biguint<128> data_i_var, data_reg_128;
	sc_uint<8> data_array[16], data_reg_var[16];

#define assign_array_to_128() \
{  \
	data_reg_128.range(127,120)=data_reg_var[0]; \
	data_reg_128.range(119,112)=data_reg_var[1]; \
	data_reg_128.range(111,104)=data_reg_var[2]; \
	data_reg_128.range(103,96)=data_reg_var[3]; \
	data_reg_128.range(95,88)=data_reg_var[4]; \
	data_reg_128.range(87,80)=data_reg_var[5]; \
	data_reg_128.range(79,72)=data_reg_var[6]; \
	data_reg_128.range(71,64)=data_reg_var[7]; \
	data_reg_128.range(63,56)=data_reg_var[8]; \
	data_reg_128.range(55,48)=data_reg_var[9]; \
	data_reg_128.range(47,40)=data_reg_var[10]; \
	data_reg_128.range(39,32)=data_reg_var[11]; \
	data_reg_128.range(31,24)=data_reg_var[12]; \
	data_reg_128.range(23,16)=data_reg_var[13]; \
	data_reg_128.range(15,8)=data_reg_var[14]; \
	data_reg_128.range(7,0)=data_reg_var[15]; \
}

#define shift_array_to_128() \
{  \
	data_reg_128.range(127,120)=data_reg_var[0]; \
	data_reg_128.range(119,112)=data_reg_var[5]; \
	data_reg_128.range(111,104)=data_reg_var[10]; \
	data_reg_128.range(103,96)=data_reg_var[15]; \
	data_reg_128.range(95,88)=data_reg_var[4]; \
	data_reg_128.range(87,80)=data_reg_var[9]; \
	data_reg_128.range(79,72)=data_reg_var[14]; \
	data_reg_128.range(71,64)=data_reg_var[3]; \
	data_reg_128.range(63,56)=data_reg_var[8]; \
	data_reg_128.range(55,48)=data_reg_var[13]; \
	data_reg_128.range(47,40)=data_reg_var[2]; \
	data_reg_128.range(39,32)=data_reg_var[7]; \
	data_reg_128.range(31,24)=data_reg_var[12]; \
	data_reg_128.range(23,16)=data_reg_var[1]; \
	data_reg_128.range(15,8)=data_reg_var[6]; \
	data_reg_128.range(7,0)=data_reg_var[11]; \
}

#define invert_shift_array_to_128() \
{  \
	data_reg_128.range(127,120)=data_reg_var[0]; \
	data_reg_128.range(119,112)=data_reg_var[13]; \
	data_reg_128.range(111,104)=data_reg_var[10]; \
	data_reg_128.range(103,96)=data_reg_var[7]; \
	data_reg_128.range(95,88)=data_reg_var[4]; \
	data_reg_128.range(87,80)=data_reg_var[1]; \
	data_reg_128.range(79,72)=data_reg_var[14]; \
	data_reg_128.range(71,64)=data_reg_var[11]; \
	data_reg_128.range(63,56)=data_reg_var[8]; \
	data_reg_128.range(55,48)=data_reg_var[5]; \
	data_reg_128.range(47,40)=data_reg_var[2]; \
	data_reg_128.range(39,32)=data_reg_var[15]; \
	data_reg_128.range(31,24)=data_reg_var[12]; \
	data_reg_128.range(23,16)=data_reg_var[9]; \
	data_reg_128.range(15,8)=data_reg_var[6]; \
	data_reg_128.range(7,0)=data_reg_var[3]; \
}

	data_i_var = data_i.read();

	data_array[0] = data_i_var.range(127, 120);
	data_array[1] = data_i_var.range(119, 112);
	data_array[2] = data_i_var.range(111, 104);
	data_array[3] = data_i_var.range(103, 96);
	data_array[4] = data_i_var.range(95, 88);
	data_array[5] = data_i_var.range(87, 80);
	data_array[6] = data_i_var.range(79, 72);
	data_array[7] = data_i_var.range(71, 64);
	data_array[8] = data_i_var.range(63, 56);
	data_array[9] = data_i_var.range(55, 48);
	data_array[10] = data_i_var.range(47, 40);
	data_array[11] = data_i_var.range(39, 32);
	data_array[12] = data_i_var.range(31, 24);
	data_array[13] = data_i_var.range(23, 16);
	data_array[14] = data_i_var.range(15, 8);
	data_array[15] = data_i_var.range(7, 0);

	data_reg_var[0] = data_reg.read().range(127, 120);
	data_reg_var[1] = data_reg.read().range(119, 112);
	data_reg_var[2] = data_reg.read().range(111, 104);
	data_reg_var[3] = data_reg.read().range(103, 96);
	data_reg_var[4] = data_reg.read().range(95, 88);
	data_reg_var[5] = data_reg.read().range(87, 80);
	data_reg_var[6] = data_reg.read().range(79, 72);
	data_reg_var[7] = data_reg.read().range(71, 64);
	data_reg_var[8] = data_reg.read().range(63, 56);
	data_reg_var[9] = data_reg.read().range(55, 48);
	data_reg_var[10] = data_reg.read().range(47, 40);
	data_reg_var[11] = data_reg.read().range(39, 32);
	data_reg_var[12] = data_reg.read().range(31, 24);
	data_reg_var[13] = data_reg.read().range(23, 16);
	data_reg_var[14] = data_reg.read().range(15, 8);
	data_reg_var[15] = data_reg.read().range(7, 0);


	sbox_decrypt_o.write(decrypt_i.read());
	sbox_data_o.write(0);
	next_state.write(state.read());
	next_data_reg.write(data_reg.read());

	next_ready_o.write(0);
	data_o.write(data_reg.read());

    uint8_t state_tmp = state.read();
    bool start_i_tmp;
    bool decrypt_i_tmp;
    //LCOV_EXCL_BR_STOP

	switch (state_tmp)
	{
		case 0:
            start_i_tmp = start_i.read();//LCOV_EXCL_BR_LINE
			if (start_i_tmp)
			{
                //LCOV_EXCL_BR_START
				sbox_data_o.write(data_array[0]);
				next_state.write(1);
                //LCOV_EXCL_BR_STOP
			}
			break;
		case 16:
            //LCOV_EXCL_BR_START
			data_reg_var[15] = sbox_data_i.read();
            decrypt_i_tmp = decrypt_i.read();
            //LCOV_EXCL_BR_STOP
			//Make shift rows stage
			switch (decrypt_i_tmp)
			{
				case 0:
					shift_array_to_128();//LCOV_EXCL_BR_LINE
					break;
				case 1:
					invert_shift_array_to_128();//LCOV_EXCL_BR_LINE
					break;
			}
            //LCOV_EXCL_BR_START
			next_data_reg.write(data_reg_128);
			next_ready_o.write(1);
			next_state.write(0);
            //LCOV_EXCL_BR_STOP
			break;
		default:
            //LCOV_EXCL_BR_START
			sbox_data_o.write(data_array[(int)state.read()]);
			data_reg_var[(int)state.read()-1] = sbox_data_i.read();
			assign_array_to_128();
			next_data_reg.write(data_reg_128);
			next_state.write(state.read() + 1);
            //LCOV_EXCL_BR_STOP
			break;
	}
}//LCOV_EXCL_BR_LINE

void subbytes::registers()
{
    bool reset_tmp = reset.read();//LCOV_EXCL_BR_LINE
	if (!reset_tmp)
	{
        //LCOV_EXCL_BR_START
		data_reg.write(0);
		state.write(0);
		ready_o.write(0);
        //LCOV_EXCL_BR_STOP
	} else {
        //LCOV_EXCL_BR_START
		data_reg.write(next_data_reg.read());
		state.write(next_state.read());
		ready_o.write(next_ready_o.read());
        //LCOV_EXCL_BR_STOP
	}
}//LCOV_EXCL_BR_LINE
