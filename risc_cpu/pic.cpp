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

  pic.cpp -- Programmable Interrupt Unit.

  Original Author: Martin Wang, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#include "systemc.h"
#include "pic.h"

void pic::entry(){
  bool ireq0_tmp;
  bool ireq1_tmp;
  bool ireq2_tmp;
  bool ireq3_tmp;
  bool intack_cpu_tmp, cs_tmp;

  //LCOV_EXCL_BR_START
  ireq0_tmp = ireq0.read();
  ireq1_tmp = ireq1.read();
  ireq2_tmp = ireq2.read();
  ireq3_tmp = ireq3.read();
  intack_cpu_tmp = intack_cpu.read();
  cs_tmp = cs.read();
  //LCOV_EXCL_BR_STOP

  if (ireq0_tmp == true) {
    intreq.write(true);//LCOV_EXCL_BR_LINE
    vectno.write(0);//LCOV_EXCL_BR_LINE
  } else if (ireq1_tmp == true) {
    intreq.write(true);//LCOV_EXCL_BR_LINE
    vectno.write(1);//LCOV_EXCL_BR_LINE
  } else if (ireq2_tmp == true) {
    intreq.write(true);//LCOV_EXCL_BR_LINE
    vectno.write(2);//LCOV_EXCL_BR_LINE
  } else if (ireq3_tmp == true) {
    intreq.write(true);//LCOV_EXCL_BR_LINE
    vectno.write(2);//LCOV_EXCL_BR_LINE
  }

  if ((intack_cpu_tmp == true) && (cs_tmp == true)) {
    intreq.write(false);//LCOV_EXCL_BR_LINE
  }
}
