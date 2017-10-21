//////////////////////////////////////////////////////////////////////
////                                                              ////
////  AES mixcolum module implementation                          ////
////                                                              ////
////  This file is part of the SystemC AES                        ////
////                                                              ////
////  Description:                                                ////
////  Mixcolum stage implementation for AES algorithm             ////
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
// $Log: mixcolum.cpp,v $
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

#include "mixcolum.h"

void mixcolum::mux()
{
  bool decrypt_i_tmp = decrypt_i.read();//LCOV_EXCL_BR_LINE

  if(decrypt_i_tmp)
    outmux.write(outy.read());//LCOV_EXCL_BR_LINE
  else
    outmux.write(outx.read());//LCOV_EXCL_BR_LINE
}

void mixcolum::mixcol()
{
    //LCOV_EXCL_BR_START
	sc_biguint<128> data_i_var;
	sc_uint<32> aux;
	sc_biguint<128> data_reg_var;

	data_i_var = data_i.read();
	data_reg_var = data_reg.read();
	next_data_reg.write(data_reg.read());
	next_state.write(state.read());

	mix_word.write(0);

	next_ready_o.write(0);
	next_data_o.write(data_o_reg.read());

    uint8_t state_tmp = state.read();
    bool start_i_tmp = start_i.read();
    //LCOV_EXCL_BR_STOP

	switch (state_tmp)
	{
		case 0:
			if (start_i_tmp)
			{
                //LCOV_EXCL_BR_START
				aux = data_i_var.range(127, 96);
				mix_word.write(aux);
				data_reg_var.range(127, 96) = outmux.read();
				next_data_reg.write(data_reg_var);
				next_state.write(1);
                //LCOV_EXCL_BR_STOP
			}
			break;
		case 1:
            //LCOV_EXCL_BR_START
			aux = data_i_var.range(95, 64);
			mix_word.write(aux);
			data_reg_var.range(95, 64) = outmux.read();
			next_data_reg.write(data_reg_var);
			next_state.write(2);
            //LCOV_EXCL_BR_STOP
			break;
		case 2:
            //LCOV_EXCL_BR_START
			aux = data_i_var.range(63, 32);
			mix_word.write(aux);
			data_reg_var.range(63, 32) = outmux.read();
			next_data_reg.write(data_reg_var);
			next_state.write(3);
            //LCOV_EXCL_BR_STOP
			break;
		case 3:
            //LCOV_EXCL_BR_START
			aux = data_i_var.range(31, 0);
			mix_word.write(aux);
			data_reg_var.range(31, 0) = outmux.read();
			next_data_o.write(data_reg_var);
			next_ready_o.write(1);
			next_state.write(0);
            //LCOV_EXCL_BR_STOP
			break;
		default:
			break;
	}
}

void mixcolum::registers()
{
    bool reset_tmp = reset.read();//LCOV_EXCL_BR_LINE

	if (!reset_tmp)
	{
        //LCOV_EXCL_BR_START
		data_reg.write(0);
		state.write(0);
		ready_o.write(0);
		data_o_reg.write(0);
        //LCOV_EXCL_BR_STOP
	}
	else
	{
        //LCOV_EXCL_BR_START
		data_reg.write(next_data_reg.read());
		state.write(next_state.read());
		ready_o.write(next_ready_o.read());
		data_o_reg.write(next_data_o.read());
        //LCOV_EXCL_BR_STOP
	}
}

void mixcolum::assign_data_o()
{
	data_o.write(data_o_reg.read());//LCOV_EXCL_BR_LINE
}//LCOV_EXCL_BR_LINE
