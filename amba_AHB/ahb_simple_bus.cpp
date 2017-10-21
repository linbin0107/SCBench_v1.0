#include "ahb_simple_bus.h"

void ahb_simple_bus::arbitrate()
{
  /* Currently Grant is being given to the master with lower index
   * we are not preempting the onging txn in this arbiter
   * even if higher priority master sends req */
  DEBUG_OUT("arbitrate BEGIN");
  /* TODO mp: i had to change this as UPPAAL does not support numbers
   * > 2^16-1 by default. This is now depending on the number of
   * masters which should work fine.
   * for(unsigned short j = 0; j < 16; j++) */
  for(unsigned short j = 0; j < MASTER_N; j++)
  {
  unsigned short temp = 1 << j;
  if(m_BusRequest_Register & temp)
  {
    if(m_InService_Register == 0)
    {
      m_InService_Register= temp;
      current_req_master=j;
      //clear the jth bit in m_BusRequest_Register now
      //m_BusRequest_Register &= (~temp); clear is done via BUS_REQ_STOP
      m_BusRequest_Register -= temp;
    }
  }
  }

  DEBUG_OUT("arbitrate END");
}

void ahb_simple_bus::bus_grant()
{
  DEBUG_OUT("bus_grant BEGIN");
  m_grant_time = SMALL_DELAY;
  m_grant_phase = BUS_GRANT;
  tlm::tlm_sync_enum retval = tlm::TLM_ACCEPTED;
  arbitrate();

  //LCOV_EXCL_BR_START
  cout << HEADER << "Sending the Bus-grant signal to the master[" <<
       current_req_master << "]" << std::endl;
  //LCOV_EXCL_BR_STOP

  switch(current_req_master) {
    #if MASTER_N>0
      case 0:
      retval = slave_sock0->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
    #if MASTER_N>1
      case 1:
      retval = slave_sock1->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
    #if MASTER_N>2
      case 2:
      retval = slave_sock2->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
       default:
      //LCOV_EXCL_BR_START
      cout << HEADER << "ERROR master " <<
           current_req_master << " does not exist." << endl;
      //LCOV_EXCL_BR_STOP
  }

  DEBUG_OUT("Master returned " << retval);
  //assert(retval == tlm::TLM_ACCEPTED);
  DEBUG_OUT("bus_grant END");
}

void ahb_simple_bus::bus_ungrant()
{
  //LCOV_EXCL_BR_START
  DEBUG_OUT("bus_ungrant BEGIN");
  cout << HEADER << "Sending the Bus- Ungrant signal to \
       the master[" << current_req_master << "]" << endl;
  DEBUG_OUT("m_BusRequest_Register = " << m_BusRequest_Register);
  //LCOV_EXCL_BR_STOP
  //tlm::tlm_generic_payload trans;
  //trans.set_command(tlm::TLM_IGNORE_COMMAND);

  m_grant_time = SMALL_DELAY;
  m_grant_phase = BUS_UNGRANT;
  m_InService_Register = 0;
  tlm::tlm_sync_enum retval= tlm::TLM_ACCEPTED;
  switch(current_req_master) {
    #if MASTER_N>0
    case 0:
      retval = slave_sock0->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
    #if MASTER_N>1
    case 1:
      retval = slave_sock1->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
    #if MASTER_N>2
    case 2:
      retval = slave_sock2->nb_transport_bw(*m_grant_txn, m_grant_phase, m_grant_time);
      break;
    #endif
    default:
      //LCOV_EXCL_BR_START
      cout << HEADER << "ERROR master " <<
          current_req_master << " does not exist." << endl;
      //LCOV_EXCL_BR_STOP
  }

  //assert(retval == tlm::TLM_ACCEPTED);
  outstanding_txn_count = 0;
  if(m_BusRequest_Register != 0) {
    DEBUG_OUT("notify bus_grant");
    bus_grant_ev.notify(); //ungrant is already delayed
  }

  DEBUG_OUT("bus_ungrant END");
}

//Needed because of the socket interfaces. Should never be invoked.
void ahb_simple_bus::b_transport(data &tran, sc_time &t)
{
  //assert(0 && "not implemented yet.");
}

bool ahb_simple_bus::get_direct_mem_ptr(data &tran, tlm::tlm_dmi &dmi_data)
{
  //assert(0 && "not implemented yet.");
  return false;
}

unsigned int ahb_simple_bus::get_index(sc_dt::uint64 address)
{
  DEBUG_OUT("get_index for address " << address);
  for(unsigned int i=0; i < SLAVE_N; i=i+1) {
        if (address >= i * MEM_SIZE && address < (i+1) * MEM_SIZE)
    return i;
    }
    //assert(0 && "address is not in this range");
    return 0;
}

void ahb_simple_bus::invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range)
{
  //assert(0 && "not implemented yet.");
}


tlm::tlm_sync_enum ahb_simple_bus::nb_transport_bw(data &tran, int &phase, sc_core::sc_time &t)
{
  DEBUG_OUT("nb_transport_bw BEGIN");

  if (phase == END_DATA || phase == tlm::BEGIN_RESP|| phase == DATA_RETRY){
    if (phase == DATA_RETRY || tran.response_status != tlm::TLM_OK_RESPONSE){ //happens one cycle before HREADY
        m_drop_cycle=1;
    } else {
        outstanding_data = NULL; //outstanding data is only reset here if we have a single cycle resp
    }

    end_data_ev.notify(m_req_sample_time); //this will get us behind any BEGIN_REQ call
  }

  unsigned int *size;
  size = tran.burst_size;
  if (*size == 0) {
    size = &m_default_size;
  }

  data_count++;
  if(data_count == (tran.data_length / (*size)))//-1))
  {
    data_count = 0;
  }

  tlm::tlm_sync_enum retval = tlm::TLM_ACCEPTED;

  switch(current_data_master) {
    #if MASTER_N>0
       case 0:
      retval = slave_sock0->nb_transport_bw(tran, phase, t);
      break;
    #endif
    #if MASTER_N>1
       case 1:
      retval = slave_sock1->nb_transport_bw(tran, phase, t);
      break;
    #endif
    #if MASTER_N>2
       case 2:
      retval = slave_sock2->nb_transport_bw(tran, phase, t);
      break;
    #endif
      default:
      //LCOV_EXCL_BR_START
      cout << HEADER << "ERROR master " <<
          current_data_master << " does not exist." << endl;
      //LCOV_EXCL_BR_STOP
  }

  DEBUG_OUT("nb_transport_bw END");

  return retval;
}

tlm::tlm_sync_enum ahb_simple_bus::nb_transport_fw(data &tran, int &phase, sc_core::sc_time &t)
{
  DEBUG_OUT("nb_transport_fw BEGIN, phase = " <<
        PHASE_OUT(phase) << ", delay = " << t <<
        ", outstanding_data = " <<
        (outstanding_data == NULL ? "null" : "not null"));

  unsigned int index = tran.amba_id;
  int addr = tran.address;
  int slave_index = get_index(addr);

  DEBUG_OUT("addr = " << addr << ", slave_index = " <<
            slave_index << ", index = " << index);

  unsigned short temp = 0;
  tlm::tlm_sync_enum retval = tlm::TLM_ACCEPTED;
  if (phase == tlm::BEGIN_REQ )
  {// add the split master id so that the slave can know abt it
    tran.amba_id = current_req_master;
  }

  if (phase == BUS_REQ)
  {//check is any other high priority master already going on with transaction
    temp = 0;
    temp = 1 << index;
    DEBUG_OUT("m_BusRequest_Register = " <<
             m_BusRequest_Register << ", temp = " << temp);

    m_BusRequest_Register = (m_BusRequest_Register | temp);
    DEBUG_OUT("m_BusRequest_Register = " << m_BusRequest_Register);

    if(outstanding_txn_count == 0)//so no txn is in progress
    {
      DEBUG_OUT("notify bus_grant with delay " << m_bus_req_sample_time);
            bus_grant_ev.notify(m_bus_req_sample_time);
      DEBUG_OUT("nb_transport_fw END");

            return tlm::TLM_ACCEPTED;
    } else {
      DEBUG_OUT("nb_transport_fw END");
      return tlm::TLM_ACCEPTED;
    }

  } else if (phase == BUS_REQ_STOP)
  {//clear our request
    temp = 0;
    temp = 1 << index;
    //TODO mp: UPPAAL does not support bitwise negation,
    // so we subtract temp from m_BusRequest_Register
    DEBUG_OUT("m_BusRequest_Register = " << m_BusRequest_Register);
    m_BusRequest_Register -= temp;
    DEBUG_OUT("m_BusRequest_Register = " << m_BusRequest_Register);
    //temp = ~temp;
    //m_BusRequest_Register &= temp;

    return tlm::TLM_ACCEPTED;

  } else if (phase == tlm::BEGIN_REQ) {
    int *type;
    type = tran.trans_type;

    if(*type == NON_SEQUENTIAL)
    {
      //ok this is the first BEGIN_REQ/NON_SEQ, then set the outstanding txn
      unsigned int *size;
      size = tran.burst_size;
      if (*size == 0) {
        size = &m_default_size;
      }
      outstanding_txn_count = tran.data_length/(*size);
    }

    if (outstanding_data != NULL)
    {
      //the last data-phase is not complete so buffer this request phase
      outstanding_req = &tran;
      //save type if we do not forward immediately
      m_outstanding_req_type = *type;
      DEBUG_OUT("nb_transport_fw END");

      return tlm::TLM_ACCEPTED;
    } else {
      //last data-phase is done, so send this request too
      retval = tlm::TLM_ACCEPTED;
      switch (slave_index) {
        #if AHB_SLAVE>0
        case 0:
          retval = master_sock_ahb0->nb_transport_fw(tran, phase, t);
          break;
        #endif
        #if AHB_SLAVE>1
        case 1:
          retval = master_sock_ahb1->nb_transport_fw(tran, phase, t);
          break;
        #endif
        #if TLM_SLAVE>0
        case AHB_SLAVE:
          retval = master_sock_tlm0->nb_transport_fw(tran, phase, t);
          break;
        #endif
        #if TLM_SLAVE>1
        case AHB_SLAVE+1:
          retval = master_sock_tlm1->nb_transport_fw(tran, phase, t);
          break;
        #endif
        default:
          //LCOV_EXCL_BR_START
          cout << HEADER << "ERROR unknown slave_index " <<
               slave_index << "!" << endl;
          //LCOV_EXCL_BR_STOP
      }

      //assert(retval == tlm::TLM_UPDATED && phase == tlm::END_REQ);
      if (*type == NON_SEQUENTIAL || *type == SEQUENTIAL){
        outstanding_data = &tran;
        if (data_count == 0)
        {//this is the first data-phase
          current_data_master = current_req_master;
        }
      }

      if (*type == IDLE){
        outstanding_txn_count = 0; //burst termination
        data_count = 0;
      } else {
         //ok so Req has completed, so lets decrement the outstanding_txn_count
         outstanding_txn_count--;
      }

      if(outstanding_txn_count == 0)
      {
        bus_ungrant_ev.notify(m_bus_req_sample_time);
      }

      DEBUG_OUT("nb_transport_fw END");

      return retval;
    }

  } else if(phase == BEGIN_DATA)
  {
    //if(outstanding_data!=NULL)
     //assert(outstanding_data == &tran);

    // just for this simple example: assuming that the slave will
        // always respond with TLM_ACCEPTED for BEGIN_DATA
    retval = tlm::TLM_ACCEPTED;
    switch (slave_index) {
      #if AHB_SLAVE>0
      case 0:
        retval = master_sock_ahb0->nb_transport_fw(*outstanding_data, phase, t);
        break;
      #endif
      #if AHB_SLAVE>1
      case 1:
        retval = master_sock_ahb1->nb_transport_fw(*outstanding_data, phase, t);
        break;
      #endif
      #if TLM_SLAVE>0
      case AHB_SLAVE:
        retval = master_sock_tlm0->nb_transport_fw(*outstanding_data, phase, t);
        break;
      #endif
      #if TLM_SLAVE>1
      case AHB_SLAVE+1:
        retval = master_sock_tlm1->nb_transport_fw(*outstanding_data, phase, t);
        break;
      #endif
      default:
        //LCOV_EXCL_BR_START
        cout << HEADER << "ERROR unknown slave_index " <<
                     slave_index << "!" << endl;
        //LCOV_EXCL_BR_STOP
    }

    DEBUG_OUT("master_sock->nb_transport returned " << retval <<
                  ", phase is " << PHASE_OUT(phase));

         if (retval == tlm::TLM_UPDATED) {
            // it seems as if we are calling bw from fw, but since
            // there is a PEQ in between that is not the case
           nb_transport_bw(*outstanding_data, phase, t);
    }

    DEBUG_OUT("nb_transport_fw END");

    return tlm::TLM_ACCEPTED;

  } else {
    //assert("phase not considered in this example");
    DEBUG_OUT("nb_transport_fw END");
    return tlm::TLM_ACCEPTED;
  }
}

void ahb_simple_bus::pendingReqProcess()
{
  DEBUG_OUT("pendingReqProcess BEGIN");
    if (m_drop_cycle == 1) {
    //drop the non HREADY cycle
    m_drop_cycle = 2;
    // TODO mp: in our first model we state that we neither
        // support next_trigger nor posedge_event and therefore
        // cannot handle this statement. We ignored it.
    //next_trigger(clk.posedge_event());
    return;
  } else if (m_drop_cycle == 2) {
    //get us to a point where the master has potentially sent the IDLE
    m_drop_cycle = 3;
    // TODO mp: in our first model we state that we do not
        // support next_trigger and therefore cannot handle
        //  this statement. We ignored it.
    //next_trigger(m_req_sample_time);
    return;
  } else if (m_drop_cycle == 3) {
    m_drop_cycle = 0;
    outstanding_data = NULL;
  }

  if(outstanding_req != NULL)
  {
    //LCOV_EXCL_BR_START
    cout << HEADER << "Sending the outstanding request \
             to the slave,nb_fw, phase: BEGIN_REQ" << endl;
    //LCOV_EXCL_BR_STOP

         int *type_ext;
    type_ext = outstanding_req->trans_type;
         *type_ext = m_outstanding_req_type; //restore type

         if (*type_ext == IDLE)
    {
      outstanding_txn_count = 1;
      data_count = 0;
    }//so it will drop to zero and terminate the burst

    unsigned int addr = outstanding_req->address;
    unsigned int index = get_index(addr);
    outstanding_req->amba_id = current_req_master;

    int ph = tlm::BEGIN_REQ;
    sc_core::sc_time delay = SMALL_DELAY;//LCOV_EXCL_BR_LINE
    tlm::tlm_sync_enum retval = tlm::TLM_ACCEPTED;
    switch (index) {
      #if AHB_SLAVE>0
      case 0:
        retval = master_sock_ahb0->nb_transport_fw(*outstanding_req, ph, delay);//LCOV_EXCL_BR_LINE
        break;
      #endif
      #if AHB_SLAVE>1
      case 1:
        retval = master_sock_ahb1->nb_transport_fw(*outstanding_req, ph, delay);//LCOV_EXCL_BR_LINE
        break;
      #endif
      #if TLM_SLAVE>0
      case AHB_SLAVE:
        retval = master_sock_tlm0->nb_transport_fw(*outstanding_req, ph, delay);//LCOV_EXCL_BR_LINE
        break;
      #endif
      #if TLM_SLAVE>1
      case AHB_SLAVE+1:
        retval = master_sock_tlm1->nb_transport_fw(*outstanding_req, ph, delay);//LCOV_EXCL_BR_LINE
        break;
      #endif
      default:
        //LCOV_EXCL_BR_START
        cout << HEADER << "ERROR unknown slave_index " <<
             index << "!" << endl;
        //LCOV_EXCL_BR_STOP
    }

    //assert(retval == tlm::TLM_UPDATED);
    //send master the END_REQ phase
    ph = tlm::END_REQ;

    //LCOV_EXCL_BR_START
    cout << HEADER << "Returning the right returnval \
        to master, nb_bw, ph:END_REQ" << endl;
    //LCOV_EXCL_BR_STOP

    switch (current_req_master) {
      #if MASTER_N>0
         case 0:
        retval = slave_sock0->nb_transport_bw(*outstanding_req, ph, delay);
        break;
      #endif
      #if MASTER_N>1
         case 1:
        retval = slave_sock1->nb_transport_bw(*outstanding_req, ph, delay);
        break;
      #endif
      #if MASTER_N>2
         case 2:
        retval = slave_sock2->nb_transport_bw(*outstanding_req, ph, delay);
        break;
      #endif
      default:
        //LCOV_EXCL_BR_START
        cout << HEADER << "ERROR master " <<
            current_req_master << " does not exist." << endl;
        //LCOV_EXCL_BR_STOP
    }
    //assert(retval == tlm::TLM_ACCEPTED);

    outstanding_txn_count--;
    if(outstanding_txn_count == 0)
    {
      bus_ungrant_ev.notify(m_bus_req_sample_time);
    }

    outstanding_data = outstanding_req ;
    current_data_master = current_req_master;
    outstanding_req = NULL;
  }

  DEBUG_OUT("pendingReqProcess END");
}

unsigned int ahb_simple_bus::transport_dbg(data& tran)
{
  //assert(0 && "not implemented yet.");
  return 0;
}
