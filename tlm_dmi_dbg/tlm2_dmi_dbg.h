
// Filename: tlm2_dmi_dbg.h

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

// Version 2  18-June-2008 - updated for TLM-2.0
// Version 3   3-July-2008 - bug fix: call dmi_data.init()
// Version 4  12-Jan-2009  - fix bug in transport_dbg
// Version 5  26-Sep-2009  - fix bug with set_end_address

// Getting Started with TLM-2.0, Tutorial Example 2

// Shows the direct memory interfaces and the DMI hint.
// Shows the debug transaction interface
// Shows the proper use of response status

#ifndef _TLM2_DMI_DBG_H_
#define _TLM2_DMI_DBG_H_

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

  // *********************************************
  // TLM-2 backward DMI method
  // *********************************************
  virtual void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                         sc_dt::uint64 end_range);

  bool dmi_ptr_valid;
  tlm::tlm_dmi dmi_data;
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
      mem[i] = 0xAA000000 | (rand() % 256);

    SC_THREAD(invalidation_process);
  }

  // TLM-2 blocking transport method
  virtual void b_transport(tlm::tlm_generic_payload& trans,
                           sc_time& delay );

  // *********************************************
  // TLM-2 forward DMI method
  // *********************************************

  virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
                                  tlm::tlm_dmi& dmi_data);

  void invalidation_process();

  // *********************************************
  // TLM-2 debug transport method
  // *********************************************
  virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

  int mem[SIZE];
};

SC_MODULE(Top)
{
  Initiator *initiator;
  Memory    *memory;

  SC_CTOR(Top)
  {
    // Instantiate components
    initiator = new Initiator("initiator");
    memory    = new Memory   ("memory");

    // One initiator is bound directly to one target with no
    // intervening bus

    // Bind initiator socket to target socket
    initiator->socket.bind(memory->socket);
  }
//LCOV_EXCL_BR_STOP

#endif
};
