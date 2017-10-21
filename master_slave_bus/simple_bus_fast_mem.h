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

  simple_bus_fast_mem.h : The memory (slave) without wait states.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef __simple_bus_fast_mem_h
#define __simple_bus_fast_mem_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"


//LCOV_EXCL_START
//LCOV_EXCL_BR_START
class simple_bus_fast_mem
  : public simple_bus_slave_if
  , public sc_module
{
public:
  // constructor
  simple_bus_fast_mem(sc_module_name name_
		      , unsigned int start_address
		      , unsigned int end_address)
    : sc_module(name_)
    , m_start_address(start_address)
    , m_end_address(end_address)
  {
    sc_assert(m_start_address <= m_end_address);
    sc_assert((m_end_address-m_start_address+1)%4 == 0);
    unsigned int size = (m_end_address-m_start_address+1)/4;
    MEM = new int [size];
    for (unsigned int i = 0; i < size; ++i)
      MEM[i] = 0;
  }

  // destructor
  ~simple_bus_fast_mem()
  {
    if (MEM)
      delete [] MEM;

    MEM = (int *)0;
  }

  // direct Slave Interface
  bool direct_read(int *data, unsigned int address);
  bool direct_write(int *data, unsigned int address);

  // Slave Interface
  simple_bus_status read(int *data, unsigned int address);
  simple_bus_status write(int *data, unsigned int address);

  unsigned int start_address() const;
  unsigned int end_address() const;

private:
  int * MEM;
  unsigned int m_start_address;
  unsigned int m_end_address;

}; // end class simple_bus_fast_mem
//LCOV_EXCL_STOP
//LCOV_EXCL_BR_STOP

#endif
