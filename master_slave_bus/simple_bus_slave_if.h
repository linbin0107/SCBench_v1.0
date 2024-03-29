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

  simple_bus_slave_if.h : The Slave interface.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef __simple_bus_slave_if_h
#define __simple_bus_slave_if_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_direct_if.h"


//LCOV_EXCL_START
//LCOV_EXCL_BR_START
class simple_bus_slave_if
  : public simple_bus_direct_if
{
public:
  // Slave interface
  virtual simple_bus_status read(int *data, unsigned int address) = 0;
  virtual simple_bus_status write(int *data, unsigned int address) = 0;

  virtual unsigned int start_address() const = 0;
  virtual unsigned int end_address() const = 0;

}; // end class simple_bus_slave_if
//LCOV_EXCL_STOP
//LCOV_EXCL_BR_STOP

#endif
