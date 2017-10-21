//////////////////////////////////////////////////////////////////////
////                                                              ////
////  sc_fifo to sc_signal adapter                                ////
////                                                              ////
////  This file is part of the SystemC AES                        ////
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
// $Log: adapt.h,v $
// Revision 1.1  2007-10-13 14:56:14  blancn
// *** empty log message ***
//
// Revision 1.1  2005/02/14 11:18:31  jcastillo
// Moved
//
// Revision 1.2  2004/08/30 14:44:43  jcastillo
// Code Formater used to give better appearance to SystemC code
//
// Revision 1.1.1.1  2004/07/05 09:46:22  jcastillo
// First import
//

#include "systemc.h"

//LCOV_EXCL_BR_START
SC_MODULE(adapter)
{
	sc_in<bool> clk;
	sc_in<bool> rt_ready_i;
	sc_in<sc_biguint<128> > rt_aes_data_i;

	sc_fifo_out<sc_biguint<128> > rt_aes_data_o;

	void adapt()
	{
		while (1)
		{
			wait(clk->posedge_event());
			if (rt_ready_i.read())
				rt_aes_data_o.write(rt_aes_data_i.read());
		}
	}

	SC_CTOR(adapter)
	{
		SC_THREAD(adapt);
	}
};
//LCOV_EXCL_BR_STOP