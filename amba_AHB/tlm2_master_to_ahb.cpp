#include "tlm2_master_to_ahb.h"

void tlm2_master_to_ahb::busGranted()
{
  DEBUG_OUT("busGranted BEGIN");
  //assert(current_trans != NULL && "Operation on NULL transaction");
  if(m_txn_count == 0) {
    startRequest();
  } else if(m_txn_count < (current_trans->data_length)) {// / (BUSWIDTH / 8)))
    resumeRequest();
  }

  DEBUG_OUT("busGranted END");
}

void tlm2_master_to_ahb::busNotGranted()
{
  DEBUG_OUT("busNotGranted BEGIN");
  sc_core::sc_time delay = SC_ZERO_TIME;//LCOV_EXCL_BR_LINE
  //various ahb signals

  if(m_bus_req_pending == false)
  {
    /* if the previous rquest has been rejected
     * set the transaction attributes here */
    //LCOV_EXCL_BR_START
    cout << HEADER << "Sending the Bus-Request \
             to the arbiter, phase: BUS_REQ" << endl;

    int ph = BUS_REQ;
    tlm::tlm_sync_enum retval = master_socket->nb_transport_fw(*current_trans, ph, delay);
    //LCOV_EXCL_BR_STOP

    switch(retval)
    {
      case tlm::TLM_UPDATED:
      {
        if(ph == BUS_GRANT)
        {
          m_bus_granted = true;
          m_bus_req_pending = false;
          //LCOV_EXCL_BR_START
          cout << HEADER << "phase: BUS_GRANT, \
               tlm_sync_enum:TLM_UPDATED" << endl;
          //LCOV_EXCL_BR_STOP
        }
        else if(ph == BUS_UNGRANT)
        {
          m_bus_granted = false;
          //LCOV_EXCL_BR_START
          cout << HEADER << "phase: BUS_UNGRANT, \
               tlm_sync_enum:TLM_UPDATED" << endl;
          //LCOV_EXCL_BR_STOP
        }
        break;
      }
      case tlm::TLM_ACCEPTED:
      {
        //assert(ph == BUS_REQ);
        m_bus_req_pending = true;

        cout << HEADER << "tlm_sync_enum: TLM_ACCEPTED" << endl;//LCOV_EXCL_BR_LINE

        break;
      }
      default:
      {
        //assert("Wrong tlm_sync_enum returned");
      }
    }
  }//else wait for the response of previous request

  DEBUG_OUT("busNotGranted END");
}

void tlm2_master_to_ahb::b_transport(tlmdata &tran, sc_core::sc_time &t)
{
  DEBUG_OUT("b_transport BEGIN");

  unsigned int cmd = tran.command;
  int  addr = tran.address;
  unsigned int *ptr = tran.data_ptr;
  unsigned int len = tran.data_length;
  unsigned int wid = tran.streaming_width;
  int current_trans_type = SEQUENTIAL;
  unsigned int current_burst_size = BURST_SIZE;

  tran.response_status = tlm::TLM_INCOMPLETE_RESPONSE;
  //  if (addr+len > stg.get_size())
  //  {
  //    trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
  //    return;
  //  }
  if (wid < len)
  {
    tran.response_status = tlm::TLM_BURST_ERROR_RESPONSE;
    DEBUG_OUT("b_transport END");
    return;
  }

  DEBUG_OUT("Received command " << ((cmd == tlm::TLM_WRITE_COMMAND) ? "WRITE" : "READ") << ".");

  switch (cmd) {
  case tlm::TLM_WRITE_COMMAND:
    current_Address = addr;
    current_DataLength = len;
    current_Command = cmd;

    //TODO mp: here master_socket.get_transaction() was used in the original model instead of new.
    current_trans = new data();
    current_trans->address = addr;
    //TODO mp: this is new, as we don't get the index of the socket via nb_transport_fw anymore
    current_trans->amba_id = master_id - 1;
    current_trans->data_length = len;
    current_trans->streaming_width = wid;
    current_trans->command = cmd;
    current_trans->data_ptr = ptr;
    current_trans->trans_type = &current_trans_type;
    current_trans->burst_size = &current_burst_size;

    //LCOV_EXCL_BR_START
    DEBUG_OUT("Waiting until data is processed.");
    wait(b_transport_finished_ev);

    cout << HEADER << "release_transaction" << endl;
    //LCOV_EXCL_BR_STOP

    //release transaction
    delete current_trans;
    current_trans = NULL;

    //next request can be started.
    current_DataLength = 0;
    m_txn_count = 0;

    //stg.write(ptr, (uint32_t)addr, 8*len);
    tran.response_status = tlm::TLM_OK_RESPONSE;
    break;

  case tlm::TLM_READ_COMMAND:
    current_Address = addr;
    current_DataLength = len;
    current_Command = cmd;
    //stg.read(ptr, (uint32_t)addr, 8*len);

    //TODO mp: here we used master_socket.get_transaction().
    current_trans = new data();
    current_trans->address = current_Address;
        //TODO mp: this is new, as we don't get the index of the socket via nb_transport_fw anymore
    current_trans->amba_id = master_id - 1;
    current_trans->data_length = current_DataLength;
    current_trans->streaming_width = current_DataLength;
    current_trans->command = current_Command;
    current_trans->data_ptr = ptr;
    current_trans->trans_type = &current_trans_type;
    current_trans->burst_size = &current_burst_size;

    //LCOV_EXCL_BR_START
    cout << HEADER << "Waiting until data is processed." << endl;
    wait(b_transport_finished_ev);

    cout << HEADER << "release_transaction" << endl;
    //LCOV_EXCL_BR_STOP

    //release transaction
    delete current_trans;
    current_trans = NULL;

    //next request can be started.
    current_DataLength = 0;
    m_txn_count = 0;

    tran.response_status = tlm::TLM_OK_RESPONSE;

    break;
  default:
    tran.response_status = tlm::TLM_GENERIC_ERROR_RESPONSE;
    break;
  }

  DEBUG_OUT("b_transport END");
}

void tlm2_master_to_ahb::clock_tick()
{
  //dataProcess has to be before requestProcess. see specification.
  dataProcess();
  requestProcess();
}

void tlm2_master_to_ahb::dataProcess()
{
  DEBUG_OUT( "dataProcess BEGIN");
  // / (BUSWIDTH / 8)) // all transaction has been done.
  if(m_data_count == current_DataLength)
  {
    DEBUG_OUT( "dataProcess END");
    return;
  }

  //ok we have broken transaction,wait until first Non-Seq txn
  if(m_restart_pending_req)
  {
    DEBUG_OUT( "dataProcess END");
    return;
  }

  if(current_trans != NULL) //new transaction available
  {
    if(current_trans->command == tlm::TLM_WRITE_COMMAND)
    {
      DEBUG_OUT("cmd = WRITE_COMMAND, burst_count = " <<
           current_trans->data_length << ", m_data_pending = " <<
           m_data_pending << ", m_data_count = " << m_data_count <<
           ", first_req_accepted = " << (m_txn_count == 1 &&
           (m_request_pending == false)) << ", sec_req_available = " <<
           (m_txn_count > 1 && m_data_count < current_trans->data_length));

      unsigned int burst_count = current_trans->data_length; ///(BUSWIDTH/8);

      bool first_req_accepted = (m_txn_count == 1 && (m_request_pending == false));
            // safe because END_REQ==END_DATA
      bool sec_req_available = (m_txn_count > 1 && m_data_count < burst_count);

      if( sec_req_available || first_req_accepted)
      {
        if(!m_data_pending)
          sendData();
      }
    }
  }

  DEBUG_OUT( "dataProcess END");
}

bool tlm2_master_to_ahb::get_direct_mem_ptr(tlmdata &trans, tlm::tlm_dmi & dmi_data)
{
  //TODO: create logic
  //assert(0 && "not implemented yet.");
  return false;
}

void tlm2_master_to_ahb::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
{
  //TODO: create logic
  //assert(0 && "not implemented yet.");
}

/* TODO mp: it seems as if the amba sockets in the original
 * model have a built-in delay. We can emulate this by using
 * a payload event queue. The whole content of the original
 * nb_transport_bw is moved into the peq_cb and we simply
 * return TLM_ACCEPTED after enqueueing the trans and phase
 * into the peq. */
tlm::tlm_sync_enum tlm2_master_to_ahb::nb_transport_bw(data &tran, int &phase, sc_core::sc_time &t)
{
  DEBUG_OUT("peq_notify with phase " << PHASE_OUT(phase) <<
             " (" << phase << "), delay " << t);

  m_peq.notify(tran, phase, t);

  return tlm::TLM_ACCEPTED;
}

tlm::tlm_sync_enum tlm2_master_to_ahb::nb_transport_fw(tlmdata &tran, int &phase, sc_core::sc_time &t)
{
  //TODO: create logic for nb_transport_fw
  //assert(0 && "not implemented yet.");

  return tlm::TLM_COMPLETED;
}

/* TODO mp: it seems as if the amba sockets in the original
 * model have a built-in delay. We can emulate this by using
 * a payload event queue. This callback method contains the
 * whole content of nb_transport_bw in the original model. */
void tlm2_master_to_ahb::peq_cb(data &tran, const int &phase)
{
  DEBUG_OUT( "nb_transport_bw BEGIN");
  DEBUG_OUT("peq_cb BEGIN");

  unsigned int *size;
  unsigned int burst_len = 0;

  if (phase == tlm::BEGIN_RESP)
  {
    receiveData(tran);
    DEBUG_OUT("nb_transport_bw END");

    return;
  }
  else if(phase == tlm::END_REQ)
  {
    cout << HEADER << "nb_bw, ph: END_REQ" << endl;//LCOV_EXCL_BR_LINE
    m_request_pending = false;

  }
  else if(phase == END_DATA)
  {
    cout << HEADER << "nb_bw, ph: END_DATA" << endl;//LCOV_EXCL_BR_LINE
    //This has been returned in case of write transaction
    size = tran.burst_size;
    burst_len = tran.data_length / (*size);

    m_data_pending = false;

    //LCOV_EXCL_BR_START
    cout << HEADER << "nb_bw, m_data_count: " << m_data_count <<
            ", bust_len: " << burst_len << endl;
    //LCOV_EXCL_BR_STOP

    if(m_data_count == burst_len)
    {
      b_transport_finished_ev.notify();
    }
    else
    {
      dataProcess();
    }
  }
  else if (phase == BUS_GRANT)
  {
    cout << HEADER << "nb_bw, ph:BUS_GRANT" << endl;//LCOV_EXCL_BR_LINE
    m_bus_granted = true;
    m_bus_req_pending = false;

    requestProcess();
  }
  else if (phase == BUS_UNGRANT)
  {
    cout << HEADER << "nb_bw, ph: BUS_UNGRANT" << endl;//LCOV_EXCL_BR_LINE
    //bus is ungranted from the master
    m_bus_granted = false;
    m_bus_req_pending = false;
    m_data_pending = false;
    m_request_pending = false;

    if(m_txn_count < burst_len)
    {//this ungrant has come in between,so preserve the state of current trans
      current_trans = &tran;
      //ok set the flag to remind master to restart this transaction again later
      m_restart_pending_req = true;
    }
  }
  else
  {//if ph== DATA_SPLIT/DATA_RETRY/RESET_ASSERT/RESET_DEASSERT
    //assert("Phase not implemented in example.");
  }
  DEBUG_OUT("peq_cb END");
  DEBUG_OUT( "nb_transport_bw END");
}


void tlm2_master_to_ahb::receiveData(data &tran)
{
  DEBUG_OUT("receiveData BEGIN");

  unsigned int *size;
  unsigned int burst_len = 0;
  m_data_count++;

  //LCOV_EXCL_BR_START
  cout << HEADER << "nb_bw, ph:BEGIN_RESP" << endl;
  cout << HEADER << "Read transaction received, \
       data-burst-count:" << m_data_count << endl;
  //LCOV_EXCL_BR_STOP

  size = tran.burst_size;
  burst_len = tran.data_length / (*size);
  unsigned int *data_ptr = tran.data_ptr;

  //LCOV_EXCL_BR_START
  cout << HEADER << "READ: data is " << (*data_ptr) <<
       ", m_data_count is " <<  m_data_count <<
       ", burst_len is " << burst_len  << endl;
  //LCOV_EXCL_BR_STOP

  if(m_data_count == burst_len) {
    b_transport_finished_ev.notify();
    m_data_pending = false;
  } else //by MK.
    m_data_pending = true;

  DEBUG_OUT("receiveData END");
}

void tlm2_master_to_ahb::resumeRequest()
{
  DEBUG_OUT("resumeRequest BEGIN");
  //various ahb signals
  int *m_trans_type;

  if(!m_request_pending )
  {
    m_txn_count++;
    //LCOV_EXCL_BR_START
    cout << HEADER << "Command: " <<
        ((current_trans->command == tlm::TLM_WRITE_COMMAND) ?
        "WRITE COMMAND" : "READ COMMAND") <<
        ",transfer:" << m_txn_count << ", Address Phase";
    //LCOV_EXCL_BR_STOP

    if(m_restart_pending_req)
    {//is this the transaction which was broken earlier due to bus_ungrant in middle
      m_trans_type = current_trans->trans_type;
      *m_trans_type = NON_SEQUENTIAL;
      m_restart_pending_req = false;
      cout << ", type: Non_Sequential";//LCOV_EXCL_BR_LINE
    }
    else
    {
      m_trans_type = current_trans->trans_type;
      *m_trans_type = SEQUENTIAL;
      cout << ", type: Sequential";//LCOV_EXCL_BR_LINE
    }

    //LCOV_EXCL_BR_START
    cout << endl;

    sc_core::sc_time delay = SMALL_DELAY;
    int ph = tlm::BEGIN_REQ;

    cout << HEADER << "nb_fw, phase: " << PHASE_OUT(ph) << endl;

    tlm::tlm_sync_enum retval = master_socket->nb_transport_fw(*current_trans, ph, delay);
    //LCOV_EXCL_BR_STOP
    m_request_pending = true;
    if(current_trans->command == tlm::TLM_READ_COMMAND) {
      m_data_pending = true;
    }

    switch(retval)
    {
      case tlm::TLM_UPDATED:
      {
        //assert(ph == tlm::END_REQ && "Unexpected phase" );
        //LCOV_EXCL_BR_START
        cout << HEADER << "retval = TLM_UPDATED, phase: " <<
                     PHASE_OUT(ph) << endl;
        //LCOV_EXCL_BR_STOP
        m_request_pending = false;
        break;
      }
      case tlm::TLM_ACCEPTED:
      {
        //assert(ph == tlm::BEGIN_REQ && "Unexpected phase");
        //LCOV_EXCL_BR_START
        cout << HEADER << "retval = TLM_ACCEPTED, phase: " <<
                     PHASE_OUT(ph) << endl;
        //LCOV_EXCL_BR_STOP
        break;
      }
      default:
      {
        //assert("Wrong tlm_sync_enum returned");
      }
    }
  }

  DEBUG_OUT("resumeRequest END");
}

void tlm2_master_to_ahb::requestProcess()
{
  DEBUG_OUT("requestProcess BEGIN");

  if(m_txn_count == current_DataLength) /// (BUSWIDTH / 8))) // || (m_bus_req_pending==true))
  {
    DEBUG_OUT("requestProcess END");

    return;//all request phases have been done
  }

  if(current_trans == NULL)
  {
    //send the bus_request to the arbiter first
    //TODO mp: here master_socket.get_transaction() was used in the original model instead of new.
    current_trans = new data();
    //current_trans = master_socket.get_transaction();
  }

  if(!m_bus_granted)
  {//if the bus has not been granted
    busNotGranted();
  }
  else //m_bus_granted==true
  {
    busGranted();
  }

  DEBUG_OUT("requestProcess END");
}

void tlm2_master_to_ahb::sendData()
{
  DEBUG_OUT("sendData BEGIN");
  // do we have previous data ack to recieve
  m_data_count++;

  //LCOV_EXCL_BR_START
  cout << HEADER << "Write Command, data transfer: " <<
         m_data_count << endl;
  //LCOV_EXCL_BR_STOP

  unsigned int *size;
  size = current_trans->burst_size;

  unsigned int burst_len;
  //unsigned int *data_ptr;

  if(*size != 0)
  {
    burst_len = current_trans->data_length / *size;
    //data_ptr = current_trans->data_ptr;
  }
  else
  {
    burst_len= 1;
    //data_ptr = current_trans->data_ptr;
  }

  //LCOV_EXCL_BR_START
  cout << HEADER << "nb_fw, phase:BEGIN_DATA" << endl;
  sc_core::sc_time delay = SMALL_DELAY;
  int ph = BEGIN_DATA;

  tlm::tlm_sync_enum retval = master_socket->nb_transport_fw(*current_trans, ph, delay);
  //LCOV_EXCL_BR_STOP
  m_data_pending = true;
  switch(retval)
  {
    case tlm::TLM_UPDATED:
    {
      //assert(ph == END_DATA);
      //LCOV_EXCL_BR_START
      cout << HEADER << "returnval = TLM_UPDATED, phase: END_DATA" << endl;
      //LCOV_EXCL_BR_STOP
      if(m_data_count == burst_len)
      {
        b_transport_finished_ev.notify();//LCOV_EXCL_BR_LINE
      }
      m_data_pending = false;

      break;
    }
    case tlm::TLM_ACCEPTED:
    {
      //assert(ph == BEGIN_DATA);
      cout << HEADER << "returnval=TLM_ACCEPTED" << endl;//LCOV_EXCL_BR_LINE
      break;
    }
    default:
    {
      //assert("Wrong tlm_sync_enum returned");
    }
  }

  DEBUG_OUT("sendData END");
}

void tlm2_master_to_ahb::startRequest()
{
  //bus has just granted the control, so start the request
  DEBUG_OUT("startRequest BEGIN");

  sc_core::sc_time delay = SMALL_DELAY;//LCOV_EXCL_BR_LINE
  //various ahb signals
  int *m_trans_type;
  unsigned int *m_burst_size;

  //first transfer is always non-sequential type
  m_trans_type = current_trans->trans_type;
  *m_trans_type = NON_SEQUENTIAL;

  //set the burstsize
  m_burst_size = current_trans->burst_size;
  *m_burst_size = BURST_SIZE;

  int ph = tlm::BEGIN_REQ;
  m_txn_count++;

    //LCOV_EXCL_BR_START
  cout << HEADER << "Starting the AHB transaction" <<
         ", TYPE=" << ((current_trans->command == tlm::TLM_WRITE_COMMAND)?
         "WRITE COMMAND":"READ COMMAND") << ", BURSTSIZE="<<
         *m_burst_size << ", BURST-LENGTH="  <<
         (current_trans->data_length / *m_burst_size) <<
         ", Request phase"<< m_txn_count << ", type=Non-sequential" << endl;

  cout << HEADER << "nb_fw, phase:BEGIN_REQ" << endl;

  m_data_count=0; // now start data phase

  tlm::tlm_sync_enum retval = master_socket->nb_transport_fw(*current_trans, ph, delay);
    //LCOV_EXCL_BR_STOP
  switch(retval)
  {
    case tlm::TLM_UPDATED:
    {
      //assert(ph == tlm::END_REQ && "Unexpected phase" );
            //LCOV_EXCL_BR_START
      cout << HEADER << "phase: END_REQ, retval=TLM_UPDATED" << endl;
            //LCOV_EXCL_BR_STOP
      m_request_pending = false;
      break;
    }
    case tlm::TLM_ACCEPTED:
    {
      //assert(ph== tlm::BEGIN_REQ && "Unexpected phase");
      cout << HEADER << "retval=TLM_ACCEPTED" << endl;//LCOV_EXCL_BR_LINE
      m_request_pending = true;

      // m_data_count=0;
      // now start data phase BUGFIX: this must happen in any case and before nb_transport_fw, otherwise we get a deadlock
      break;
    }
    default:
    {
      //assert("Wrong tlm_sync_enum returned");
    }
  }

  DEBUG_OUT("startRequest END");
}

// TLM-2.0 interface method for tlm2 target socket
unsigned int tlm2_master_to_ahb::transport_dbg(tlmdata& trans)
{
  //TODO: create logic
  //assert(0 && "not implemented yet.");
  return 0;
}

