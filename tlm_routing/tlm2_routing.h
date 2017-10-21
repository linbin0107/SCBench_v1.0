
// Filename: tlm2_routing.h

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

#ifndef _TLM_ROUTING_H_
#define _TLM_ROUTING_H_

// Define the following macro to invoke an error response
// from the target
// #define INJECT_ERROR

// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

// Initiator module generating generic payload transactions
//LCOV_EXCL_BR_START
struct Initiator: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<Initiator> socket;

  SC_CTOR(Initiator)
  : socket("socket"),  // Construct and name socket
    dmi_ptr_valid(false)
  {
    // Register callbacks for incoming interface method
    // calls
    socket.register_invalidate_direct_mem_ptr(this,
           &Initiator::invalidate_direct_mem_ptr);

    SC_THREAD(thread_process);
  }

  void thread_process();

  // TLM-2 backward DMI method
  virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range);

  bool dmi_ptr_valid;
  tlm::tlm_dmi dmi_data;
};

// *********************************************
// Generic payload blocking transport router
// *********************************************
template<unsigned int N_TARGETS>
struct Router: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Router> target_socket;

  // *********************************************
  // Use tagged sockets to be able to distinguish incoming
  // backward path calls
  // *********************************************

  tlm_utils::simple_initiator_socket_tagged<Router>* initiator_socket[N_TARGETS];

  SC_CTOR(Router)
  : target_socket("target_socket")
  {
    // Register callbacks for incoming interface method
    // calls
    target_socket.register_b_transport(this, &Router::b_transport);
    target_socket.register_get_direct_mem_ptr(this, &Router::get_direct_mem_ptr);
    target_socket.register_transport_dbg(this, &Router::transport_dbg);

    for (unsigned int i = 0; i < N_TARGETS; i++)
    {
      char txt[20];
      sprintf(txt, "socket_%d", i);
      initiator_socket[i] = new tlm_utils::simple_initiator_socket_tagged<Router>(txt);

      // *********************************************
      // Register callbacks for incoming interface method
      // calls, including tags
      // *********************************************
      initiator_socket[i]->register_invalidate_direct_mem_ptr(this,
             &Router::invalidate_direct_mem_ptr, i);
    }
  }

  // ****************
  // FORWARD PATH
  // ****************

  // TLM-2 blocking transport method
  virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
  {
    sc_dt::uint64 address = trans.get_address();
    sc_dt::uint64 masked_address;
    unsigned int target_nr = decode_address( address, masked_address);

    // Modify address within transaction
    trans.set_address( masked_address );

    // Forward transaction to appropriate target
    ( *initiator_socket[target_nr] )->b_transport( trans, delay );
  }

  // TLM-2 forward DMI method
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                  tlm::tlm_dmi& dmi_data)
  {
    sc_dt::uint64 masked_address;
    unsigned int target_nr = decode_address( trans.get_address(), masked_address );
    trans.set_address( masked_address );

    bool status = ( *initiator_socket[target_nr])->get_direct_mem_ptr( trans, dmi_data );

    // Calculate DMI address of target in system address
    // space
    dmi_data.set_start_address( compose_address( target_nr, dmi_data.get_start_address() ));
    dmi_data.set_end_address  ( compose_address( target_nr, dmi_data.get_end_address() ));

    return status;
  }

  // TLM-2 debug transaction method
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
  {
    sc_dt::uint64 masked_address;
    unsigned int target_nr = decode_address( trans.get_address(), masked_address );
    trans.set_address( masked_address );

    // Forward debug transaction to appropriate target
    return ( *initiator_socket[target_nr] )->transport_dbg( trans );
  }

  // ****************
  // BACKWARD PATH
  // ****************

  // **************************
  // Tagged backward DMI method
  // **************************

  virtual void invalidate_direct_mem_ptr(int id,
                                         sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range)
  {
    // Reconstruct address range in system memory map
    sc_dt::uint64 bw_start_range = compose_address( id, start_range );
    sc_dt::uint64 bw_end_range   = compose_address( id, end_range );
    target_socket->invalidate_direct_mem_ptr(bw_start_range, bw_end_range);
  }

  // ****************
  // ROUTER INTERNALS
  // ****************

  // Simple fixed address decoding
  inline unsigned int decode_address( sc_dt::uint64 address, sc_dt::uint64& masked_address )
  {
    unsigned int target_nr = static_cast<unsigned int>( (address >> 8) & 0x3 );
    masked_address = address & 0xFF;
    return target_nr;
  }

  inline sc_dt::uint64 compose_address( unsigned int target_nr, sc_dt::uint64 address)
  {
    return (target_nr << 8) | (address & 0xFF);
  }

};


// Target module representing a simple memory
struct Memory: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Memory> socket;

  enum { SIZE = 256 };
  const sc_time LATENCY;

  SC_CTOR(Memory)
  : socket("socket"), LATENCY(10, SC_NS)
  {
    // Register callbacks for incoming interface method
    // calls
    socket.register_b_transport(this, &Memory::b_transport);
    socket.register_get_direct_mem_ptr(this, &Memory::get_direct_mem_ptr);
    socket.register_transport_dbg(this, &Memory::transport_dbg);

    // Initialize memory with random data
    for (int i = 0; i < SIZE; i++)
      mem[i] = 0xAA000000 | (mem_nr << 20) | (rand() % 256);

    ++mem_nr;
  }

  // TLM-2 blocking transport method
  virtual void b_transport(tlm::tlm_generic_payload& trans,
                           sc_time& delay);

  // TLM-2 forward DMI method
  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                  tlm::tlm_dmi& dmi_data);

  // TLM-2 debug transaction method
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  int mem[SIZE];
  static unsigned int mem_nr;
};

SC_MODULE(Top)
{
  Initiator* initiator;
  Router<4>* router;
  Memory*    memory[4];

  SC_CTOR(Top)
  {
    // Instantiate components
    initiator = new Initiator("initiator");
    router    = new Router<4>("router");
    for (int i = 0; i < 4; i++)
    {
      char txt[20];
      sprintf(txt, "memory_%d", i);
      memory[i]   = new Memory(txt);
    }

    // Bind sockets
    initiator->socket.bind( router->target_socket );
    for (int i = 0; i < 4; i++)
      router->initiator_socket[i]->bind( memory[i]->socket);
  }
};
//LCOV_EXCL_BR_STOP

#endif
