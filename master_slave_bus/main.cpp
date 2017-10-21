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

  simple_bus_main.cpp : sc_main

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "systemc.h"
#include "tb_simple_bus.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int, char **)
{
  simple_bus_test tt("testbench");

#ifdef WAVE_DUMP
  sc_trace_file *trace_file = sc_create_vcd_trace_file("simple_bus");

  sc_trace(trace_file, tt.C1, "tt.C1");
#endif

  sc_start(10000, SC_NS);

  tt.compare_results();

#ifdef WAVE_DUMP
  sc_close_vcd_trace_file(trace_file);
#endif

  return 0;
}
//LCOV_EXCL_STOP
//LCOV_EXCL_BR_STOP
