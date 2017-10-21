//////////////////////////////////////////////////////////////////////
////                                                              ////
////  AES key schedule implementation                             ////
////                                                              ////
////  This file is part of the SystemC AES                        ////
////                                                              ////
////  Description:                                                ////
////  Generate the next round key from the previous one           ////
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
// $Log: keysched.cpp,v $
// Revision 1.1  2007-10-13 14:56:14  blancn
// *** empty log message ***
//
// Revision 1.1  2005/02/14 11:18:31  jcastillo
// Moved
//
// Revision 1.2  2004/08/30 14:44:44  jcastillo
// Code Formater used to give better appearance to SystemC code
//
// Revision 1.1.1.1  2004/07/05 09:46:22  jcastillo
// First import
//


#include "keysched.h"

//Rcon ROM
void keysched::rcon()
{
    uint8_t round_i_tmp = round_i.read();//LCOV_EXCL_BR_LINE
	switch (round_i_tmp)
	{
		case 1:
			rcon_o.write(1);//LCOV_EXCL_BR_LINE
			break;
		case 2:
			rcon_o.write(2);//LCOV_EXCL_BR_LINE
			break;
		case 3:
			rcon_o.write(4);//LCOV_EXCL_BR_LINE
			break;
		case 4:
			rcon_o.write(8);//LCOV_EXCL_BR_LINE
			break;
		case 5:
			rcon_o.write(0x10);//LCOV_EXCL_BR_LINE
			break;
		case 6:
			rcon_o.write(0x20);//LCOV_EXCL_BR_LINE
			break;
		case 7:
			rcon_o.write(0x40);//LCOV_EXCL_BR_LINE
			break;
		case 8:
			rcon_o.write(0x80);//LCOV_EXCL_BR_LINE
			break;
		case 9:
			rcon_o.write(0x1B);//LCOV_EXCL_BR_LINE
			break;
		case 10:
			rcon_o.write(0x36);//LCOV_EXCL_BR_LINE
			break;
		default:
			rcon_o.write(0);//LCOV_EXCL_BR_LINE
			break;
	}
}

void keysched::generate_key()
{
    //LCOV_EXCL_BR_START
	sc_biguint<128> K_var, W_var;
	sc_uint<32> col_t;
	sc_uint<24> zero;

	zero = 0;

	col_t = col.read();
	W_var = 0;

	next_state.write(state.read());
	next_col.write(col.read());

	next_ready_o.write(0);
	next_key_reg.write(key_reg.read());
	new_key_o.write(key_reg.read());

	sbox_decrypt_o.write(0);
	sbox_access_o.write(0);
	sbox_data_o.write(0);
	K_var = last_key_i.read();

    uint8_t state_tmp = state.read();
    bool start_i_tmp = start_i.read();
    //LCOV_EXCL_BR_STOP

	switch (state_tmp)
	{
			//Substitute the bytes while rotating them
			//Four accesses to SBox are needed
		case 0:
			if (start_i_tmp)
			{
                //LCOV_EXCL_BR_START
				col_t = 0;
				sbox_access_o.write(1);
				sbox_data_o.write((sc_uint < 8 >)K_var.range(31, 24));
				next_state.write(1);
                //LCOV_EXCL_BR_STOP
			}
			break;
		case 1:
            //LCOV_EXCL_BR_START
			sbox_access_o.write(1);
			sbox_data_o.write((sc_uint < 8 >)K_var.range(23, 16));
			col_t.range(7, 0) = sbox_data_i.read();
			next_col.write(col_t);
			next_state.write(2);
            //LCOV_EXCL_BR_STOP
			break;
		case 2:
            //LCOV_EXCL_BR_START
			sbox_access_o.write(1);
			sbox_data_o.write((sc_uint < 8 >)K_var.range(15, 8));
			col_t.range(31, 24) = sbox_data_i.read();
			next_col.write(col_t);
			next_state.write(3);
            //LCOV_EXCL_BR_STOP
			break;
		case 3:
            //LCOV_EXCL_BR_START
			sbox_access_o.write(1);
			sbox_data_o.write((sc_uint < 8 >)K_var.range(7, 0));
			col_t.range(23, 16) = sbox_data_i.read();
			next_col.write(col_t);
			next_state.write(4);
            //LCOV_EXCL_BR_STOP
			break;
		case 4:
            //LCOV_EXCL_BR_START
			sbox_access_o.write(1);
			col_t.range(15, 8) = sbox_data_i.read();
			next_col.write(col_t);
			W_var.range(127, 96) = col_t ^ K_var.range(127, 96) ^ (rcon_o.read(), zero);
			W_var.range(95, 64) = W_var.range(127, 96) ^ K_var.range(95, 64);
			W_var.range(63, 32) = W_var.range(95, 64) ^ K_var.range(63, 32);
			W_var.range(31, 0) = W_var.range(63, 32) ^ K_var.range(31, 0);
			next_ready_o.write(1);
			next_key_reg.write(W_var);
			next_state.write(0);
            //LCOV_EXCL_BR_STOP
			break;

		default:
			next_state.write(0);//LCOV_EXCL_BR_LINE
			break;
	}
}

void keysched::registers()
{
    bool reset_tmp = reset.read();//LCOV_EXCL_BR_LINE
	if (!reset_tmp)
	{
        //LCOV_EXCL_BR_START
		state.write(0);
		col.write(0);
		key_reg.write(0);
		ready_o.write(0);
        //LCOV_EXCL_BR_STOP
	}
	else
	{
        //LCOV_EXCL_BR_START
		state.write(next_state.read());
		col.write(next_col.read());
		key_reg.write(next_key_reg.read());
		ready_o.write(next_ready_o.read());
        //LCOV_EXCL_BR_STOP
	}
}//LCOV_EXCL_BR_LINE
