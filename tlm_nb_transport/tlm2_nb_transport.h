
// Filename: tlm2_nb_transport.h

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

// Version 2  19-June-2008 - updated for TLM-2.0


// Getting Started with TLM-2.0, Example 4

// Shows the non-blocking transport interface with the
// generic payload and simple sockets
// Shows nb_transport used with the forward and backward
// paths
// Both components are able to accept transactions on the
// return path,
// although neither component actually uses the return path
// (TLM_UPDATED)

// Shows the Approximately Timed coding style
// Models processing delay of initiator, latency of target,
// and request and response accept delays
// Uses payload event queues to manage both timing
// annotations and internal delays

// Shows the BEGIN_REQ exclusion rule at the initiator and
// BEGIN_RESP exclusion rule at the target
// In this example, the target allows two pipelined
// transactions in-flight

// Shows an explicit memory manager and reference counting

// No use of temporal decoupling, DMI or debug transport
// Nominal use of the blocking transport interface just to
// show the simple socket b/nb adapter

#ifndef _TLM_NB_TRANSPORT_H_
#define _TLM_NB_TRANSPORT_H_


// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

#define DEBUG

//LCOV_EXCL_BR_START
// ********************************************************
// User-defined memory manager, which maintains a pool of
// transactions
// ********************************************************
class mm: public tlm::tlm_mm_interface
{
  typedef tlm::tlm_generic_payload gp_t;

public:
  mm() : free_list(0), empties(0)
  #ifdef DEBUG
  , count(0)
  #endif
  {}

  gp_t* allocate();
  void  free(gp_t* trans);

private:
  struct access
  {
    gp_t* trans;
    access* next;
    access* prev;
  };

  access* free_list;
  access* empties;

  #ifdef DEBUG
  int     count;
  #endif
};

// ********************************************************
// Initiator module generating multiple pipelined generic
// payload transactions
// ********************************************************
struct Initiator: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<Initiator> socket;

  SC_CTOR(Initiator)
  : socket("socket")  // Construct and name socket
  , request_in_progress(0)
  , m_peq(this, &Initiator::peq_cb)
  {
    // Register callbacks for incoming interface method
    // calls
    socket.register_nb_transport_bw(this, &Initiator::nb_transport_bw);

    SC_THREAD(thread_process);
  }

  void thread_process();

  // TLM-2 backward non-blocking transport method
  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans,
                                             tlm::tlm_phase& phase,
                                             sc_time& delay);

  // Payload event queue callback to handle transactions
  // from target
  // Transaction could have arrived through return path or
  // backward path
  void peq_cb(tlm::tlm_generic_payload& trans, const
              tlm::tlm_phase& phase);

  // Called on receiving BEGIN_RESP or TLM_COMPLETED
  void check_transaction(tlm::tlm_generic_payload& trans);

  mm   m_mm;
  int  data[16];
  tlm::tlm_generic_payload* request_in_progress;
  sc_event end_request_event;
  tlm_utils::peq_with_cb_and_phase<Initiator> m_peq;
};


// ********************************************************
// Target module able to handle two pipelined transactions
// ********************************************************

DECLARE_EXTENDED_PHASE(internal_ph);

struct Target: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Target> socket;

  SC_CTOR(Target)
  : socket("socket")
  , n_trans(0)
  , response_in_progress(false)
  , next_response_pending(0)
  , end_req_pending(0)
  , m_peq(this, &Target::peq_cb)
  {
    // Register callbacks for incoming interface method
    // calls
    socket.register_nb_transport_fw(this, &Target::nb_transport_fw);
  }

  // TLM-2 non-blocking transport method

  virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
                                             tlm::tlm_phase& phase,
                                             sc_time& delay);

  void peq_cb(tlm::tlm_generic_payload& trans,
              const tlm::tlm_phase& phase);

  tlm::tlm_sync_enum send_end_req(tlm::tlm_generic_payload& trans);

  void send_response(tlm::tlm_generic_payload& trans);

  int   n_trans;
  bool  response_in_progress;
  tlm::tlm_generic_payload*  next_response_pending;
  tlm::tlm_generic_payload*  end_req_pending;
  tlm_utils::peq_with_cb_and_phase<Target> m_peq;
};

SC_MODULE(Top)
{
  Initiator *initiator;
  Target    *target;

  SC_CTOR(Top)
  {
    // Instantiate components
    initiator = new Initiator("initiator");
    target    = new Target   ("target");

    // One initiator is bound directly to one target with no
    // intervening bus

    // Bind initiator socket to target socket
    initiator->socket.bind(target->socket);
  }
};
//LCOV_EXCL_BR_STOP

#endif
