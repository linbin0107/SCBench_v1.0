
// Filename: tlm2_b_transport.cpp

//----------------------------------------------------------------------
//  Copyright (c) 2007-2008 by Doulos Ltd.
//
//  Licensed under the Apache License, Version 2.0 (the
//  "License");
//  you may not use this file except in compliance with the
//  License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in
//  writing, software
//  distributed under the License is distributed on an "AS
//  IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
//  express or implied.
//  See the License for the specific language governing
//  permissions and
//  limitations under the License.
//----------------------------------------------------------------------

// Version 2  16-June-2008 - updated for TLM-2.0

// Getting Started with TLM-2.0, Tutorial Example 1

// For a full description, see
// http://www.doulos.com/knowhow/systemc/tlm2

// Shows the generic payload, sockets, and blocking
// transport interface.
// Shows the responsibilities of initiator and target with
// respect to the generic payload.
// Has only dummy implementations of the direct memory and
// debug transaction interfaces.
// Does not show the non-blocking transport interface.

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

#include "tlm2_b_transport.h"

void Initiator::thread_process()
{
  // TLM-2 generic payload transaction, reused across
  // calls to b_transport
  //LCOV_EXCL_BR_START
  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  sc_time delay = sc_time(10, SC_NS);
  //LCOV_EXCL_BR_STOP

  // Generate a random sequence of reads and writes
  for (int i = 32; i < 96; i += 4)
  {
    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);

    if (cmd == tlm::TLM_WRITE_COMMAND)
      data = 0xFF000000 | i;

    // Initialize 8 out of the 10 attributes,
    // byte_enable_length and extensions being unused
    trans->set_command( cmd );
    trans->set_address( i );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
    trans->set_data_length( 4 );
    // = data_length to indicate no streaming
    trans->set_streaming_width( 4 );
    // 0 indicates unused
    trans->set_byte_enable_ptr( 0 );
    // Mandatory initial value
    trans->set_dmi_allowed( false );
    // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

    // Blocking transport call
    socket->b_transport( *trans, delay );//LCOV_EXCL_BR_LINE

    // Initiator obliged to check response status and delay
    if ( trans->is_response_error() )
      SC_REPORT_ERROR("TLM-2", "Response error from b_transport");//LCOV_EXCL_BR_LINE

    //LCOV_EXCL_BR_START
    cout << "trans = { " << (cmd ? 'W' : 'R') << ", " <<
         hex << i << " } , data = " << hex << data <<
         " at time " << sc_time_stamp() << " delay = " <<
         delay << endl;

    // Realize the delay annotated onto the transport call
    wait(delay);
    //LCOV_EXCL_BR_STOP
  }
}

// TLM-2 blocking transport method
void Memory::b_transport(tlm::tlm_generic_payload& trans,
                         sc_time& delay)
{
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64    adr = trans.get_address() / 4;
  unsigned char*   ptr = trans.get_data_ptr();
  unsigned int     len = trans.get_data_length();
  unsigned char*   byt = trans.get_byte_enable_ptr();
  unsigned int     wid = trans.get_streaming_width();

  // Obliged to check address range and check for
  // unsupported features,
  //   i.e. byte enables, streaming, and bursts
  // Can ignore DMI hint and extensions
  // Using the SystemC report handler is an acceptable way
  // of signalling an error

  if (adr >= sc_dt::uint64(SIZE) || byt != 0 || len > 4 || wid < len)
    SC_REPORT_ERROR("TLM-2", "Target does not support \
                    given generic payload transaction");

  // Obliged to implement read and write commands
  if ( cmd == tlm::TLM_READ_COMMAND )
    memcpy(ptr, &mem[adr], len);
  else if ( cmd == tlm::TLM_WRITE_COMMAND )
    memcpy(&mem[adr], ptr, len);

  // Obliged to set response status to indicate successful
  // completion
  trans.set_response_status( tlm::TLM_OK_RESPONSE );
}//LCOV_EXCL_BR_LINE
