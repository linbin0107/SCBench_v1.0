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

  simple_bus_slow_mem.cpp : Slave : The memory (slave) with wait states.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "simple_bus_slow_mem.h"

void simple_bus_slow_mem::wait_loop()
{
  if (m_wait_count >= 0)
    m_wait_count--;
}

inline bool simple_bus_slow_mem::direct_read(int *data, unsigned int address)
{
  *data = MEM[(address - m_start_address)/4];
  return true;
}

inline bool simple_bus_slow_mem::direct_write(int *data, unsigned int address)
{
  MEM[(address - m_start_address)/4] = *data;
  return true;
}

inline simple_bus_status simple_bus_slow_mem::read(int *data
						   , unsigned int address)
{
  // accept a new call if m_wait_count < 0)
  if (m_wait_count < 0)
    {
      m_wait_count = m_nr_wait_states;
      return SIMPLE_BUS_WAIT;
    }
  if (m_wait_count == 0)
    {
      *data = MEM[(address - m_start_address)/4];
      return SIMPLE_BUS_OK;
    }
  return SIMPLE_BUS_WAIT;
}

inline simple_bus_status simple_bus_slow_mem::write(int *data
						    , unsigned int address)
{
  // accept a new call if m_wait_count < 0)
  if (m_wait_count < 0)
    {
      m_wait_count = m_nr_wait_states;
      return SIMPLE_BUS_WAIT;
    }
  if (m_wait_count == 0)
    {
      MEM[(address - m_start_address)/4] = *data;
      return SIMPLE_BUS_OK;
    }
  return SIMPLE_BUS_WAIT;
}


inline unsigned int simple_bus_slow_mem::start_address() const
{
  return m_start_address;
}

inline unsigned int simple_bus_slow_mem::end_address() const
{
  return m_end_address;
}
