
// Filename: tlm2_routing.cpp

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

// Version 2 - fix warnings that only showed up using g++
// Version 3  18-June-2008 - updated for TLM-2.0
// Version 4  12-Jan-2009  - fix bug in transport_dbg
// Version 5  26-Sep-2009  - fix bug with set_end_address


// Getting Started with TLM-2.0, Tutorial Example 3


// Shows a router modeled as an interconnect component
// between the initiator and the target
// The router decodes the address to select a target, and
// masks the address in the transaction
// Shows the router passing transport, DMI and debug
// transactions along forward and backward paths
// and doing address translation in both directions

#include "tlm2_routing.h"

void Initiator::thread_process()
{
  // TLM-2 generic payload transaction, reused across
  // calls to b_transport, DMI and debug
  //LCOV_EXCL_BR_START
  tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
  sc_time delay = sc_time(10, SC_NS);
  //LCOV_EXCL_BR_STOP

  // Generate a random sequence of reads and writes
  for (int i = 256-64; i < 256+64; i += 4)
  {
    int data;
    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
    if (cmd == tlm::TLM_WRITE_COMMAND) data = 0xFF000000 | i;

    // Use DMI if it is available
    if (dmi_ptr_valid && sc_dt::uint64(i) >= dmi_data.get_start_address()
                      && sc_dt::uint64(i) <= dmi_data.get_end_address())
    {
      // Bypass transport interface and use direct memory
      // interface
      // Implement target latency
      if ( cmd == tlm::TLM_READ_COMMAND )
      {
        assert( dmi_data.is_read_allowed() );
        memcpy(&data, dmi_data.get_dmi_ptr() + i - dmi_data.get_start_address(), 4);
        wait( dmi_data.get_read_latency());//LCOV_EXCL_BR_LINE
      }
      else if ( cmd == tlm::TLM_WRITE_COMMAND )
      {
        assert( dmi_data.is_write_allowed() );
        memcpy(dmi_data.get_dmi_ptr() + i - dmi_data.get_start_address(), &data, 4);
        wait( dmi_data.get_write_latency() );//LCOV_EXCL_BR_LINE
      }

      //LCOV_EXCL_BR_START
      cout << "DMI   = { " << (cmd ? 'W' : 'R') <<
        ", " << hex << i << " } , data = " << hex <<
        data << " at time " << sc_time_stamp() << endl;
      //LCOV_EXCL_BR_STOP
    }
    else
    {
      trans->set_command( cmd );
      trans->set_address( i );
      trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data) );
      trans->set_data_length( 4 );
      trans->set_streaming_width( 4 ); // = data_length to indicate no streaming
      trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
      trans->set_dmi_allowed( false ); // Mandatory initial value
      trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

#ifdef INJECT_ERROR
        if (i > 90) trans->set_streaming_width(2);
#endif

      // Other fields default: byte enable = 0, streaming
      // width = 0, DMI_hint = false, no extensions
      // Blocking transport call
      socket->b_transport( *trans, delay );//LCOV_EXCL_BR_LINE

      // Initiator obliged to check response status
      if ( trans->is_response_error() )
      {
        // Print response string
        //LCOV_EXCL_BR_START
        char txt[100];
        sprintf(txt, "Error from b_transport, response status = %s",
                     trans->get_response_string().c_str());
        SC_REPORT_ERROR("TLM-2", txt);
        //LCOV_EXCL_BR_STOP

      }

      // Check DMI hint
      if ( trans->is_dmi_allowed() )
      {
        // *********************************************
        // Re-use transaction object for DMI. Reset the
        // address because it could
        // have been modified by the interconnect on the
        // previous transport call
        // *********************************************

        trans->set_address( i );
        dmi_ptr_valid = socket->get_direct_mem_ptr( *trans, dmi_data );//LCOV_EXCL_BR_LINE
      }

      //LCOV_EXCL_BR_START
      cout << "trans = { " << (cmd ? 'W' : 'R') <<
        ", " << hex << i << " } , data = " << hex <<
        data << " at time " << sc_time_stamp() << endl;
      //LCOV_EXCL_BR_STOP
    }
  }

  // Use debug transaction interface to dump memory
  // contents, reusing same transaction object
  sc_dt::uint64 A = 128;
  trans->set_address(A);
  trans->set_read();
  trans->set_data_length(256);

  unsigned char* data = new unsigned char[256];//LCOV_EXCL_BR_LINE
  trans->set_data_ptr(data);

  unsigned int n_bytes = socket->transport_dbg( *trans );//LCOV_EXCL_BR_LINE

  for (unsigned int i = 0; i < n_bytes; i += 4)
  {
    //LCOV_EXCL_BR_START
    cout << "mem[" << (A + i) << "] = " <<
      *(reinterpret_cast<unsigned int*>( &data[i] )) << endl;
    //LCOV_EXCL_BR_STOP
  }

  A = 256;
  trans->set_address(A);
  trans->set_data_length(128);

  n_bytes = socket->transport_dbg( *trans );//LCOV_EXCL_BR_LINE

  for (unsigned int i = 0; i < n_bytes; i += 4)
  {
    //LCOV_EXCL_BR_START
    cout << "mem[" << (A + i) << "] = "
         << *(reinterpret_cast<unsigned int*>( &data[i] )) << endl;
    //LCOV_EXCL_BR_STOP
  }
}

// TLM-2 backward DMI method
void Initiator::invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                          sc_dt::uint64 end_range)
{
  // Ignore range and invalidate all DMI pointers
  // regardless
  dmi_ptr_valid = false;
}

// TLM-2 blocking transport method
void Memory::b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
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
  // Can ignore extensions

  // Generate the appropriate error response
  if (adr >= SIZE) {
    trans.set_response_status( tlm::TLM_ADDRESS_ERROR_RESPONSE );
    return;
  }
  if (byt != 0) {
    trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
    return;
  }
  if (len > 4 || wid < len) {
    trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
    return;
  }

  wait(delay);
  delay = SC_ZERO_TIME;

  // Obliged to implement read and write commands
  if ( cmd == tlm::TLM_READ_COMMAND )
    memcpy(ptr, &mem[adr], len);
  else if ( cmd == tlm::TLM_WRITE_COMMAND )
    memcpy(&mem[adr], ptr, len);

  // Set DMI hint to indicated that DMI is supported
  trans.set_dmi_allowed(true);

  // Obliged to set response status to indicate successful
  // completion
  trans.set_response_status( tlm::TLM_OK_RESPONSE );
}

// TLM-2 forward DMI method
bool Memory::get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                tlm::tlm_dmi& dmi_data)
{
  // Permit read and write access
  dmi_data.allow_read_write();

  // Set other details of DMI region
  dmi_data.set_dmi_ptr( reinterpret_cast<unsigned char*>( &mem[0] ) );
  dmi_data.set_start_address( 0 );
  dmi_data.set_end_address( SIZE*4-1 );
  dmi_data.set_read_latency( LATENCY );
  dmi_data.set_write_latency( LATENCY );

  return true;
}

// TLM-2 debug transaction method
unsigned int Memory::transport_dbg(tlm::tlm_generic_payload& trans)
{
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64    adr = trans.get_address() / 4;
  unsigned char*   ptr = trans.get_data_ptr();
  unsigned int     len = trans.get_data_length();

  // Calculate the number of bytes to be actually copied
  unsigned int num_bytes = (len < (SIZE - adr) * 4) ? len : (SIZE - adr) * 4;

  if ( cmd == tlm::TLM_READ_COMMAND )
    memcpy(ptr, &mem[adr], num_bytes);
  else if ( cmd == tlm::TLM_WRITE_COMMAND )
    memcpy(&mem[adr], ptr, num_bytes);

  return num_bytes;
}

unsigned int Memory::mem_nr = 0;//LCOV_EXCL_BR_LINE
