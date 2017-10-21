//////////////////////////////////////////////////////////////////////
////                                                              ////
////  AES Top module                                              ////
////                                                              ////
////  This file is part of the SystemC AES                        ////
////                                                              ////
////  Description:                                                ////
////  TOP module                                                  ////
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
// $Log: aes.cpp,v $
// Revision 1.1  2007-10-13 14:56:14  blancn
// *** empty log message ***
//
// Revision 1.2  2005/03/16 18:12:25  jcastillo
//
// Style modifications
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

#include "aes.h"

void aes::registers()
{
	if (!reset.read())
	{
        //LCOV_EXCL_BR_START
		state.write(IDLE);
		ready_o.write(0);
		round.write(0);
		addroundkey_round.write(0);
		addroundkey_data_reg.write(0);
		addroundkey_ready_o.write(0);
		addroundkey_start_i.write(0);
		first_round_reg.write(0);
        //LCOV_EXCL_BR_STOP
	}
	else
	{
        //LCOV_EXCL_BR_START
		state.write(next_state.read());
		ready_o.write(next_ready_o.read());
		round.write(next_round.read());
		addroundkey_round.write(next_addroundkey_round.read());
		addroundkey_data_reg.write(next_addroundkey_data_reg.read());
		addroundkey_ready_o.write(next_addroundkey_ready_o);
		first_round_reg.write(next_first_round_reg.read());
		addroundkey_start_i.write(next_addroundkey_start_i.read());
        //LCOV_EXCL_BR_STOP
	}
}


void aes::addroundkey()
{
    //LCOV_EXCL_BR_START
	sc_biguint<128> data_var, round_data_var, round_key_var;

	round_data_var = addroundkey_data_reg.read();
	next_addroundkey_data_reg.write(addroundkey_data_reg.read());
	next_addroundkey_ready_o.write(0);
	next_addroundkey_round.write(addroundkey_round.read());
	addroundkey_data_o.write(addroundkey_data_reg.read());

    //LCOV_EXCL_BR_STOP
	if (addroundkey_round.read() == 1 || addroundkey_round.read() == 0)
		keysched_last_key_i.write(key_i.read());//LCOV_EXCL_BR_LINE
	else
		keysched_last_key_i.write(keysched_new_key_o.read());//LCOV_EXCL_BR_LINE

    //LCOV_EXCL_BR_START
	keysched_start_i.write(0);

	keysched_round_i.write(addroundkey_round.read());
    //LCOV_EXCL_BR_STOP
	if (round.read() == 0 && addroundkey_start_i.read())
	{
		//Take the input and xor them with data if round==0;
        //LCOV_EXCL_BR_START
		data_var = addroundkey_data_i.read();
		round_key_var = key_i.read();
		round_data_var = round_key_var ^ data_var;
		next_addroundkey_data_reg.write(round_data_var);
		next_addroundkey_ready_o.write(1);
        //LCOV_EXCL_BR_STOP
	}
	else if (addroundkey_start_i.read() && round.read() != 0)
	{
        //LCOV_EXCL_BR_START
		keysched_last_key_i.write(key_i.read());
		keysched_start_i.write(1);
		keysched_round_i.write(1);
		next_addroundkey_round.write(1);
        //LCOV_EXCL_BR_STOP
	}
	else if (addroundkey_round.read() != round.read() && keysched_ready_o.read())
	{
        //LCOV_EXCL_BR_START
		next_addroundkey_round.write(addroundkey_round.read() + 1);
		keysched_last_key_i.write(keysched_new_key_o.read());
		keysched_start_i.write(1);
		keysched_round_i.write(addroundkey_round.read() + 1);
        //LCOV_EXCL_BR_STOP
	}
	else if (addroundkey_round.read() == round.read() && keysched_ready_o.read())
	{
        //LCOV_EXCL_BR_START
  		data_var = addroundkey_data_i.read();
		round_key_var = keysched_new_key_o.read();
		round_data_var = round_key_var ^ data_var;

		next_addroundkey_data_reg.write(round_data_var);
		next_addroundkey_ready_o.write(1);
		next_addroundkey_round.write(0);
        //LCOV_EXCL_BR_STOP
	}
}

void aes::sbox_muxes()
{

	if (keysched_sbox_access_o.read())
	{
		sbox_decrypt_i.write(keysched_sbox_decrypt_o.read());
		sbox_data_i.write(keysched_sbox_data_o.read());
	}
	else
	{
		sbox_decrypt_i.write(subbytes_sbox_decrypt_o.read());
		sbox_data_i.write(subbytes_sbox_data_o.read());
	}
}


void aes::control()
{
    //LCOV_EXCL_BR_START
	next_state.write(state.read());
	next_round.write(round.read());
	data_o.write(addroundkey_data_o.read());
	next_ready_o.write(0);

	//To key schedule module

	next_first_round_reg.write(0);

	subbytes_data_i.write(0);
	mixcol_data_i.write(0);
	addroundkey_data_i.write(0);

	next_addroundkey_start_i.write(first_round_reg.read());
	mixcol_start_i.write((addroundkey_ready_o.read() & decrypt_i.read()
          & round.read() != 10) | (subbytes_ready_o.read() & !decrypt_i.read()));
	subbytes_start_i.write((addroundkey_ready_o.read() & !decrypt_i.read())
          | (mixcol_ready_o.read() & decrypt_i.read())
          | (addroundkey_ready_o.read() & decrypt_i.read() & round.read() == 10));

   bool decrypt_i_tmp = decrypt_i.read();
   uint8_t round_tmp = round.read();
   //LCOV_EXCL_BR_STOP

	if (decrypt_i_tmp && (round_tmp != 10))
	{
        //LCOV_EXCL_BR_START
		addroundkey_data_i.write(subbytes_data_o.read());
		subbytes_data_i.write(mixcol_data_o.read());
		mixcol_data_i.write(addroundkey_data_o.read());
        //LCOV_EXCL_BR_STOP
	}
	else if (!decrypt_i_tmp && (round_tmp != 0))
	{
        //LCOV_EXCL_BR_START
		addroundkey_data_i.write(mixcol_data_o.read());
		subbytes_data_i.write(addroundkey_data_o.read());
		mixcol_data_i.write(subbytes_data_o.read());
        //LCOV_EXCL_BR_STOP
	}
	else
	{
        //LCOV_EXCL_BR_START
		mixcol_data_i.write(subbytes_data_o.read());
		subbytes_data_i.write(addroundkey_data_o.read());
		addroundkey_data_i.write(data_i.read());
        //LCOV_EXCL_BR_STOP
	}

	switch (state.read())
	{
		case IDLE:
			if (load_i.read())
			{
				next_state.write(ROUNDS);//LCOV_EXCL_BR_LINE
				if(decrypt_i.read())
				  next_round.write(10);//LCOV_EXCL_BR_LINE
				else
				  next_round.write(0);//LCOV_EXCL_BR_LINE
				next_first_round_reg.write(1);//LCOV_EXCL_BR_LINE
			}
			break;

		case ROUNDS:

			//Counter
			if (!decrypt_i.read() && mixcol_ready_o.read())
			{
                //LCOV_EXCL_BR_START
				next_addroundkey_start_i.write(1);
				addroundkey_data_i.write(mixcol_data_o.read());
				next_round.write(round.read() + 1);
                //LCOV_EXCL_BR_STOP
			}
			else if (decrypt_i.read() && subbytes_ready_o.read())
			{
                //LCOV_EXCL_BR_START
				next_addroundkey_start_i.write(1);
				addroundkey_data_i.write(subbytes_data_o.read());
				next_round.write(round.read() - 1);
                //LCOV_EXCL_BR_STOP
			}

			//Output
			if ((round.read() == 9 && !decrypt_i.read()) || (round.read() == 0 && decrypt_i.read()))
			{
                //LCOV_EXCL_BR_START
				next_addroundkey_start_i.write(0);
				mixcol_start_i.write(0);
                //LCOV_EXCL_BR_STOP
				if (subbytes_ready_o.read())
				{
                    //LCOV_EXCL_BR_START
					addroundkey_data_i.write(subbytes_data_o.read());
					next_addroundkey_start_i.write(1);
					next_round.write(round.read() + 1);
                    //LCOV_EXCL_BR_STOP
				}
			}
			if ((round.read() == 10 && !decrypt_i.read()) || (round.read() == 0 && decrypt_i.read()))
			{
                //LCOV_EXCL_BR_START
				addroundkey_data_i.write(subbytes_data_o.read());
				subbytes_start_i.write(0);
                //LCOV_EXCL_BR_STOP
				if (addroundkey_ready_o.read())
				{
                    //LCOV_EXCL_BR_START
					next_ready_o.write(1);
					next_state.write(IDLE);
					next_addroundkey_start_i.write(0);
					next_round.write(0);
                    //LCOV_EXCL_BR_STOP
				}
			}

			break;

		default:
			next_state.write(IDLE);//LCOV_EXCL_BR_LINE
			break;
	}
}
