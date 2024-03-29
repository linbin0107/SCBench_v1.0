
// Filename: tlm2_nb_transport.cpp

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

#include "tlm2_nb_transport.h"

static ofstream fout("foo.txt");

mm::gp_t* mm::allocate()
{
  #ifdef DEBUG
    fout << "----------------------------- Called \
         allocate(), #trans = " << ++count << endl;
  #endif

  gp_t* ptr;
  if (free_list)
  {
    ptr = free_list->trans;
    empties = free_list;
    free_list = free_list->next;
  }

  else
  {
    ptr = new gp_t(this);//LCOV_EXCL_BR_LINE
  }
  return ptr;
}

void mm::free(gp_t* trans)
{
  #ifdef DEBUG
    fout << "----------------------------- Called free(), \
         #trans = " << --count << endl;
  #endif

  if (!empties)
  {
    empties = new access;
    empties->next = free_list;
    empties->prev = 0;
    if (free_list)
      free_list->prev = empties;
  }

  free_list = empties;
  free_list->trans = trans;
  empties = free_list->prev;
}

// Generate a random delay (with power-law distribution) to
// aid testing and stress the protocol
int rand_ps()
{
  int n = rand() % 100;
  n = n * n * n;
  return n / 100;
}

void Initiator::thread_process()
{
  tlm::tlm_generic_payload* trans;
  tlm::tlm_phase phase;
  sc_time delay;

  // Generate a sequence of random transactions
  for (int i = 0; i < 1000; i++)
  {
    int adr = rand();
    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
    if (cmd == tlm::TLM_WRITE_COMMAND) data[i % 16] = rand();

    // Grab a new transaction from the memory manager
    trans = m_mm.allocate();//LCOV_EXCL_BR_LINE
    trans->acquire();

    // Set all attributes except byte_enable_length and
    // extensions (unused)
    trans->set_command( cmd );
    trans->set_address( adr );
    trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data[i % 16]) );
    trans->set_data_length( 4 );
    // = data_length to indicate no streaming
    trans->set_streaming_width( 4 );
    // 0 indicates unused
    trans->set_byte_enable_ptr( 0 );
    // Mandatory initial value
    trans->set_dmi_allowed( false );
    // Mandatory initial value
    trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE );

    // Initiator must honor BEGIN_REQ/END_REQ exclusion
    // rule
    if (request_in_progress)
      wait(end_request_event);//LCOV_EXCL_BR_LINE

    request_in_progress = trans;
    phase = tlm::BEGIN_REQ;

    // Timing annotation models processing time of
    // initiator prior to call
    delay = sc_time(rand_ps(), SC_PS);//LCOV_EXCL_BR_LINE

    //LCOV_EXCL_BR_START
    fout << hex << adr << " new, cmd=" << (cmd ? 'W' : 'R')
         << ", data=" << hex << data[i % 16] <<
         " at time " << sc_time_stamp() << endl;
    //LCOV_EXCL_BR_STOP

    // Non-blocking transport call on the forward path
    tlm::tlm_sync_enum status;
    status = socket->nb_transport_fw( *trans, phase, delay);//LCOV_EXCL_BR_LINE

    // Check value returned from nb_transport_fw
    if (status == tlm::TLM_UPDATED)
    {
      // The timing annotation must be honored
      m_peq.notify( *trans, phase, delay );//LCOV_EXCL_BR_LINE
    }
    else if (status == tlm::TLM_COMPLETED)
    {
      // The completion of the transaction necessarily
      // ends the BEGIN_REQ phase
      request_in_progress = 0;

      // The target has terminated the transaction
      check_transaction( *trans );//LCOV_EXCL_BR_LINE
    }

    wait( sc_time(rand_ps(), SC_PS) );//LCOV_EXCL_BR_LINE
  }

  wait(100, SC_NS);//LCOV_EXCL_BR_LINE

  // Allocate a transaction for one final, nominal call to
  // b_transport
  trans = m_mm.allocate();//LCOV_EXCL_BR_LINE
  trans->acquire();
  trans->set_command( tlm::TLM_WRITE_COMMAND );
  trans->set_address( 0 );
  trans->set_data_ptr( reinterpret_cast<unsigned char*>(&data[0]) );
  trans->set_data_length( 4 );
  // = data_length to indicate no streaming
  trans->set_streaming_width( 4 );
  // 0 indicates unused
  trans->set_byte_enable_ptr( 0 );
   // Mandatory initial value
  trans->set_dmi_allowed( false );
  // Mandatory initial value
  trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE);

  delay = sc_time(rand_ps(), SC_PS);//LCOV_EXCL_BR_LINE

  //LCOV_EXCL_BR_START
  fout << "Calling b_transport at " << sc_time_stamp() <<
       " with delay = " << delay << endl;
  //LCOV_EXCL_BR_STOP

  // Call b_transport to demonstrate the b/nb conversion
  // by the simple_target_socket
  socket->b_transport( *trans, delay );//LCOV_EXCL_BR_LINE
  check_transaction( *trans );//LCOV_EXCL_BR_LINE
}

// TLM-2 backward non-blocking transport method
tlm::tlm_sync_enum Initiator::nb_transport_bw(tlm::tlm_generic_payload& trans,
                                            tlm::tlm_phase& phase, sc_time& delay)
{
  // The timing annotation must be honored
  m_peq.notify( trans, phase, delay );

  return tlm::TLM_ACCEPTED;
}

// Payload event queue callback to handle transactions
// from target
// Transaction could have arrived through return path or
// backward path
void Initiator::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
{
  #ifdef DEBUG
    if (phase == tlm::END_REQ){
      fout << hex << trans.get_address() <<
           " END_REQ at " << sc_time_stamp() << endl;
    } else if (phase == tlm::BEGIN_RESP) {
      fout << hex << trans.get_address() <<
           " BEGIN_RESP at " << sc_time_stamp() << endl;
    }
  #endif

  if (phase == tlm::END_REQ || (&trans == request_in_progress && phase == tlm::BEGIN_RESP))
  {
    // The end of the BEGIN_REQ phase
    request_in_progress = 0;
    end_request_event.notify();//LCOV_EXCL_BR_LINE
  }
  else if (phase == tlm::BEGIN_REQ || phase == tlm::END_RESP)
    SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by initiator");

  if (phase == tlm::BEGIN_RESP)
  {
    check_transaction( trans );//LCOV_EXCL_BR_LINE

    // Send final phase transition to target
    tlm::tlm_phase fw_phase = tlm::END_RESP;
    sc_time delay = sc_time(rand_ps(), SC_PS);//LCOV_EXCL_BR_LINE
    socket->nb_transport_fw( trans, fw_phase, delay );//LCOV_EXCL_BR_LINE
    // Ignore return value
  }
}

// Called on receiving BEGIN_RESP or TLM_COMPLETED
void Initiator::check_transaction(tlm::tlm_generic_payload& trans)
{
  if ( trans.is_response_error() )
  {
    //LCOV_EXCL_BR_START
    char txt[100];
    sprintf(txt, "Transaction returned with error, response status = %s",
                 trans.get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
    //LCOV_EXCL_BR_STOP
  }

  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64  adr = trans.get_address();
  int* ptr = reinterpret_cast<int*>( trans.get_data_ptr() );

  //LCOV_EXCL_BR_START
  fout<< hex << adr << " check, cmd=" << (cmd ? 'W' : 'R') <<
      ", data=" << hex << *ptr << " at time " <<
      sc_time_stamp() << endl;
  //LCOV_EXCL_BR_STOP

  // Allow the memory manager to free the transaction
  // object
  trans.release();
}

// TLM-2 non-blocking transport method
tlm::tlm_sync_enum Target::nb_transport_fw(tlm::tlm_generic_payload& trans,
                                           tlm::tlm_phase& phase, sc_time& delay)
{
  sc_dt::uint64    adr = trans.get_address();
  unsigned int     len = trans.get_data_length();
  unsigned char*   byt = trans.get_byte_enable_ptr();
  unsigned int     wid = trans.get_streaming_width();

  // Obliged to check the transaction attributes for
  // unsupported features
  // and to generate the appropriate error response
  if (byt != 0) {
    trans.set_response_status( tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE );
    return tlm::TLM_COMPLETED;
  }

  if (len > 4 || wid < len) {
    trans.set_response_status( tlm::TLM_BURST_ERROR_RESPONSE );
    return tlm::TLM_COMPLETED;
  }

  // Now queue the transaction until the annotated time
  // has elapsed
  m_peq.notify( trans, phase, delay);//LCOV_EXCL_BR_LINE

  return tlm::TLM_ACCEPTED;
}

void Target::peq_cb(tlm::tlm_generic_payload& trans, const tlm::tlm_phase& phase)
{
  tlm::tlm_sync_enum status;
  sc_time delay;

  switch (phase) {
  case tlm::BEGIN_REQ:

    #ifdef DEBUG
      //LCOV_EXCL_BR_START
      fout << hex << trans.get_address() <<
          " BEGIN_REQ at " << sc_time_stamp() << endl;
      //LCOV_EXCL_BR_STOP
    #endif

    // Increment the transaction reference count
    trans.acquire();

    // Put back-pressure on initiator by deferring END_REQ
    // until pipeline is clear
    if (n_trans == 2)
      end_req_pending = &trans;
    else
    {
      status = send_end_req(trans);//LCOV_EXCL_BR_LINE
      // It is questionable whether this is valid
      if (status == tlm::TLM_COMPLETED)
        break;
    }

    break;

  case tlm::END_RESP:
    // On receiving END_RESP, the target can release the
    // transaction
    // and allow other pending transactions to proceed

    #ifdef DEBUG
      //LCOV_EXCL_BR_START
      fout << hex << trans.get_address() << " END_RESP at " <<
           sc_time_stamp() << endl;
      //LCOV_EXCL_BR_STOP
    #endif

    if (!response_in_progress)
    {
      //LCOV_EXCL_BR_START
      SC_REPORT_FATAL("TLM-2", "Illegal transaction phase \
                      END_RESP received by target");
      //LCOV_EXCL_BR_STOP
    }

    trans.release();//LCOV_EXCL_BR_LINE
    n_trans--;

    // Target itself is now clear to issue the next
    // BEGIN_RESP
    response_in_progress = false;
    if (next_response_pending)
    {
      send_response( *next_response_pending );//LCOV_EXCL_BR_LINE
      next_response_pending = 0;
    }

    // ... and to unblock the initiator by issuing END_REQ
    if (end_req_pending)
    {
      status = send_end_req( *end_req_pending );//LCOV_EXCL_BR_LINE
      end_req_pending = 0;
    }

    break;

  case tlm::END_REQ:
  case tlm::BEGIN_RESP:
    //LCOV_EXCL_BR_START
    SC_REPORT_FATAL("TLM-2", "Illegal transaction phase received by target");
    //LCOV_EXCL_BR_STOP
    break;

  default:
    if (phase == internal_ph)
    {
      // Execute the read or write commands

      tlm::tlm_command cmd = trans.get_command();
      sc_dt::uint64    adr = trans.get_address();
      unsigned char*   ptr = trans.get_data_ptr();
      unsigned int     len = trans.get_data_length();

      if ( cmd == tlm::TLM_READ_COMMAND )
      {
        *reinterpret_cast<int*>(ptr) = rand();
        //LCOV_EXCL_BR_START
        fout << hex << adr << " Execute READ, data = " <<
             *reinterpret_cast<int*>(ptr) << endl;
        //LCOV_EXCL_BR_STOP
      }
      else if ( cmd == tlm::TLM_WRITE_COMMAND )
      {
        //LCOV_EXCL_BR_START
        fout << hex << adr << " Execute WRITE, data = " <<
             *reinterpret_cast<int*>(ptr) << endl;
        //LCOV_EXCL_BR_STOP
      }

      trans.set_response_status( tlm::TLM_OK_RESPONSE );

      // Target must honor BEGIN_RESP/END_RESP exclusion
      // rule
      // i.e. must not send BEGIN_RESP until receiving
      // previous END_RESP or BEGIN_REQ
      if (response_in_progress)
      {
        // Target allows only two transactions in-flight
        if (next_response_pending)
        {
          //LCOV_EXCL_BR_START
          SC_REPORT_FATAL("TLM-2", "Attempt to have two \
                          pending responses in target");
          //LCOV_EXCL_BR_STOP
        }

        next_response_pending = &trans;
      }
      else
        send_response(trans);//LCOV_EXCL_BR_LINE
      break;
    }
  }
}

tlm::tlm_sync_enum Target::send_end_req(tlm::tlm_generic_payload& trans)
{
  tlm::tlm_sync_enum status;
  tlm::tlm_phase bw_phase;
  tlm::tlm_phase int_phase = internal_ph;
  sc_time delay;

  // Queue the acceptance and the response with the
  // appropriate latency
  bw_phase = tlm::END_REQ;
  // Accept delay
  delay = sc_time(rand_ps(), SC_PS);//LCOV_EXCL_BR_LINE
  status = socket->nb_transport_bw(trans, bw_phase, delay);//LCOV_EXCL_BR_LINE
  if (status == tlm::TLM_COMPLETED)
  {
    // Transaction aborted by the initiator
    // (TLM_UPDATED cannot occur at this point in the base
    // protocol, so need not be checked)
    trans.release();//LCOV_EXCL_BR_LINE
    return status;
  }

  // Queue internal event to mark beginning of response
  //Latency
  delay = delay + sc_time(rand_ps(), SC_PS);//LCOV_EXCL_BR_LINE
  m_peq.notify( trans, int_phase, delay );//LCOV_EXCL_BR_LINE
  n_trans++;

  return status;
}

void Target::send_response(tlm::tlm_generic_payload& trans)
{
  tlm::tlm_sync_enum status;
  tlm::tlm_phase bw_phase;
  sc_time delay;

  response_in_progress = true;
  bw_phase = tlm::BEGIN_RESP;
  delay = SC_ZERO_TIME;
  status = socket->nb_transport_bw(trans, bw_phase, delay);//LCOV_EXCL_BR_LINE

  if (status == tlm::TLM_UPDATED)
  {
    // The timing annotation must be honored
    m_peq.notify(trans, bw_phase, delay);//LCOV_EXCL_BR_LINE
  }
  else if (status == tlm::TLM_COMPLETED)
  {
    // The initiator has terminated the transaction
    trans.release();//LCOV_EXCL_BR_LINE
    n_trans--;
    response_in_progress = false;
  }
}//LCOV_EXCL_BR_LINE
