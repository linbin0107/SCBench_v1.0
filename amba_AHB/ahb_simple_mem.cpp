#include "ahb_simple_mem.h"

void ahb_simple_mem::b_transport(data &tran, sc_core::sc_time &t )
{
    //assert(0 && "not implemented yet.");
}

bool ahb_simple_mem::get_direct_mem_ptr(data &tran, tlm::tlm_dmi &dmi_data)
{
    //assert(0 && "not implemented yet.");
    return false;
}

void ahb_simple_mem::invalidate_direct_mem_ptr(int start_range, int end_range)
{
    //assert(0 && "not implemented yet.");
}

/* TODO mp: it seems as if the amba sockets in the original
 * model have a built-in delay. We can emulate this by using
 * a payload event queue. The whole content of the original
 * nb_transport_fw is moved into the peq_cb and we simply
 * return TLM_ACCEPTED after enqueueing the trans and phase
 * into the peq. */
tlm::tlm_sync_enum ahb_simple_mem::nb_transport_fw(data &tran, int &phase, sc_core::sc_time &t)
{
  DEBUG_OUT("peq_notify with phase " << PHASE_OUT(phase) << ", delay " << t);
  /* TODO mp: had to add these clauses as something is
   * very strange here: in the original model the
   * ahb_simple_mem::peq_cb method (or better the original
   * nb_transport_fw) is called 1 ps AFTER the execution of
   * ahb_simple_bus::nb_transport_fw. Nevertheless the
   * assertion that phase == END_REQ is fulfilled instantly.
   * Therefore we change the phase to END_REQ if it was
   * BEGIN_REQ and return the correct return value depending
   * on the phase here. */
  m_peq.notify(tran, phase, t);
  if (phase == tlm::BEGIN_REQ) {
    phase = tlm::END_REQ;
    return tlm::TLM_UPDATED;
  } else {
    return tlm::TLM_ACCEPTED;
  }
}

/* TODO mp: it seems as if the amba sockets in the original
 * model have a built-in delay. We can emulate this by using
 * a payload event queue. This callback method contains the
 * whole content of nb_transport_fw in the original model. */
void ahb_simple_mem::peq_cb(data &tran, const int &phase)
{
  DEBUG_OUT("nb_transport_fw BEGIN");
  DEBUG_OUT("peq_cb BEGIN");

  int *type;
  unsigned int *size;

  if(phase == tlm::BEGIN_REQ)
  {
    cout << HEADER << "nb_fw, ph:BEGIN_REQ" << endl;//LCOV_EXCL_BR_LINE
    current_trans = &tran;
    type = current_trans->trans_type;
    size = current_trans->burst_size;

    if(*type == NON_SEQUENTIAL)
    {
      //save this address and the burst-length
      txn_address = current_trans->address;
      txn_burst_len = current_trans->data_length / (*size);
      counter = 0;
    }

    if(current_trans->command == tlm::TLM_READ_COMMAND)
        {
      //we have to send the data to the master
      m_data_pending = true;
    }

  } else if (phase == BEGIN_DATA) {
    cout << HEADER << "nb_fw, ph:BEGIN_DATA" << endl;//LCOV_EXCL_BR_LINE
    //we have to copy the data in the memory.
    m_data_pending = true;
  }

  DEBUG_OUT("peq_cb END");
  DEBUG_OUT("nb_transport_fw END");
}

void ahb_simple_mem::readData(unsigned int *burst_size)
{
  DEBUG_OUT("readData BEGIN");

  cout << HEADER << "Read transaction received";//LCOV_EXCL_BR_LINE

  unsigned int *data_ptr = current_trans->data_ptr;

  int t1;
  t1 = (txn_address - baseAddr) + (*burst_size - 1);
  t1 = t1 % MEM_SIZE;
  *data_ptr = MEM[t1];
  //LCOV_EXCL_BR_START
  cout << HEADER << "READ: MEM[" << t1 <<
      "] = " << MEM[t1] << endl;
  //LCOV_EXCL_BR_STOP

  current_trans->data_ptr = data_ptr;

  int ph = tlm::BEGIN_RESP;
  //LCOV_EXCL_BR_START
  sc_core::sc_time delay = SC_ZERO_TIME;
  cout << HEADER << "Sending nb_bw, BEGIN_RESP";
    current_trans->response_status = tlm::TLM_OK_RESPONSE;

  tlm::tlm_sync_enum retval = slave_socket->nb_transport_bw(*current_trans, ph, delay);
  //assert((retval == tlm::TLM_ACCEPTED) && (ph == tlm::BEGIN_RESP));
  //LCOV_EXCL_BR_STOP

  m_data_pending = false;
  counter++;

  DEBUG_OUT("readData END");
}

void ahb_simple_mem::responseProcess()
{
  DEBUG_OUT("responseProcess BEGIN");

  if(current_trans != NULL)
  {
    unsigned int cmd = current_trans->command;
    unsigned int *burst_size = current_trans->burst_size;

    if(cmd == tlm::TLM_WRITE_COMMAND)
    {
      if (m_data_pending == true)
      {
        if(txn_burst_len != counter)
        {
          writeData(burst_size);
          txn_address += *burst_size;
        }
        else if (txn_burst_len == counter)
          counter = 0;
      }
    }
    else if (cmd == tlm::TLM_READ_COMMAND)
    {
      if (m_data_pending == true)
      {
        if(txn_burst_len != counter)
        {
          readData(burst_size);
          txn_address += *burst_size;
        }
        else if (txn_burst_len == counter)
          counter=0;
      }
    }
    else
    {
      //assert(0 && "UNKNOWN COMMAND!");
    }
  }

  DEBUG_OUT("responseProcess END");
}

unsigned int ahb_simple_mem::transport_dbg(data& tran)
{
    //assert(0 && "not implemented yet.");
    return 0;
}

void ahb_simple_mem::writeData(unsigned int * burst_size)
{
  DEBUG_OUT("writeData BEGIN");

  cout << HEADER << "Write transaction received" << endl;//LCOV_EXCL_BR_LINE

  unsigned int *data_ptr = current_trans->data_ptr;

  int t1;
  t1 = (txn_address - baseAddr) + (*burst_size -1);
  t1 = t1 % MEM_SIZE;
  MEM[t1] = data_ptr[t1];
  //LCOV_EXCL_BR_START
  cout << HEADER << "WRITE: MEM[" << t1 <<
            "] = " << data_ptr[t1] << endl;

  int ph = END_DATA;
  sc_core::sc_time delay = SMALL_DELAY;

  cout << HEADER << "Sending nb_bw, ph = END_DATA" << endl;

  current_trans->response_status = tlm::TLM_OK_RESPONSE;

  tlm::tlm_sync_enum retval = slave_socket->nb_transport_bw(*current_trans, ph, delay);
  //assert((retval == tlm::TLM_ACCEPTED) && (ph == END_DATA) && "invalid phase returned");
  //LCOV_EXCL_BR_STOP

  m_data_pending = false;
  counter++;

  DEBUG_OUT("writeData END");
}
