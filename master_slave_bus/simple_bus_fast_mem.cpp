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

  simple_bus_fast_mem.cpp : The memory (slave) without wait states.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "simple_bus_fast_mem.h"

inline bool simple_bus_fast_mem::direct_read(int *data, unsigned int address)
{
  return (read(data, address) == SIMPLE_BUS_OK);
}

inline bool simple_bus_fast_mem::direct_write(int *data, unsigned int address)
{
  return (write(data, address) == SIMPLE_BUS_OK);
}

inline simple_bus_status simple_bus_fast_mem::read(int *data
						   , unsigned int address)
{
  *data = MEM[(address - m_start_address)/4];
  return SIMPLE_BUS_OK;
}

inline simple_bus_status simple_bus_fast_mem::write(int *data
						    , unsigned int address)
{
  MEM[(address - m_start_address)/4] = *data;
  return SIMPLE_BUS_OK;
}

inline unsigned int simple_bus_fast_mem::start_address() const
{
  return m_start_address;
}

inline unsigned int simple_bus_fast_mem::end_address() const
{
  return m_end_address;
}
