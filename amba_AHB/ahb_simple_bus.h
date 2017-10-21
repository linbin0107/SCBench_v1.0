#ifndef AHB_SIMPLE_BUS_H_
#define AHB_SIMPLE_BUS_H_

#include "data.h"
#include "defines.h"

//Set MASTER_N!=1 if there are multiple slaves and arbiter should have multimaster socket
//set SLAVE_N!=1 if there are multiple masters and arbiter should have multisalve socket
//LCOV_EXCL_BR_START
class ahb_simple_bus: public sc_core::sc_module,
                      public tlm::tlm_bw_transport_if<data>,
                      public tlm::tlm_fw_transport_if<data>
{
public:
  #if AHB_SLAVE>0
    tlm::tlm_initiator_socket<BUSWIDTH, data, 0> master_sock_ahb0;
  #endif
  #if AHB_SLAVE>1
    tlm::tlm_initiator_socket<BUSWIDTH, data, 0> master_sock_ahb1;
  #endif

  #if TLM_SLAVE>0
  tlm::tlm_initiator_socket<BUSWIDTH, data, 0> master_sock_tlm0;
  #endif
  #if TLM_SLAVE>1
  tlm::tlm_initiator_socket<BUSWIDTH, data, 0> master_sock_tlm1;
  #endif

  #if MASTER_N>0
  tlm::tlm_target_socket<BUSWIDTH, data, 0> slave_sock0;
  #endif
  #if MASTER_N>1
  tlm::tlm_target_socket<BUSWIDTH, data, 0> slave_sock1;
  #endif
  #if MASTER_N>2
  tlm::tlm_target_socket<BUSWIDTH, data, 0> slave_sock2;
  #endif

  data *outstanding_req;
  data *outstanding_data;
  unsigned int outstanding_txn_count;
  unsigned int data_count;
  sc_core::sc_event end_data_ev;
  sc_core::sc_event bus_ungrant_ev;
  sc_core::sc_event bus_grant_ev;
  unsigned int current_req_master;
  unsigned int current_data_master;
  unsigned short m_BusRequest_Register;
  unsigned short m_InService_Register;

  data *m_grant_txn;
  int m_grant_phase;
  sc_core::sc_time m_grant_time;

  sc_core::sc_time m_bus_req_sample_time, m_req_sample_time;
  unsigned int m_drop_cycle;
  unsigned int m_default_size;
  int m_outstanding_req_type;

  void arbitrate();
  void bus_ungrant();
  void bus_grant();
  void b_transport(data &tran, sc_core::sc_time &t);
  bool get_direct_mem_ptr(data &tran, tlm::tlm_dmi &dmi_data);
  unsigned int get_index(sc_dt::uint64 address);
  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
  tlm::tlm_sync_enum nb_transport_bw(data &tran, int &phase, sc_core::sc_time &t);
  tlm::tlm_sync_enum nb_transport_fw(data &tran, int &phase, sc_core::sc_time &t);
  void pendingReqProcess();
  unsigned int transport_dbg(data &tran);

  SC_HAS_PROCESS(ahb_simple_bus);

  ahb_simple_bus(sc_core::sc_module_name nm) : sc_module(nm),
    #if AHB_SLAVE > 0
    master_sock_ahb0("ahb_arbiter_master_socket_ahb0"),
    #endif
    #if AHB_SLAVE > 1
    master_sock_ahb1("ahb_arbiter_master_socket_ahb1"),
    #endif
    #if TLM_SLAVE > 0
    master_sock_tlm0("ahb_arbiter_master_socket_tlm0"),
    #endif
    #if TLM_SLAVE > 1
    master_sock_tlm1("ahb_arbiter_master_socket_tlm1"),
    #endif
    #if MASTER_N > 0
    slave_sock0("ahb_arbiter_slave_socket0"),
    #endif
    #if MASTER_N > 1
    slave_sock1("ahb_arbiter_slave_socket1"),
    #endif
    #if MASTER_N > 2
    slave_sock2("ahb_arbiter_slave_socket2"),
    #endif
    outstanding_req(NULL),
    outstanding_data(NULL),
    outstanding_txn_count(0),
    data_count(0),
    current_req_master(0),
    current_data_master(0),
    m_BusRequest_Register(0),
    m_InService_Register(0),
    m_drop_cycle(0)
  {
    /* TODO mp: what is the default value of m_grant_txn? We
     * have to allocate some memory as it is never allocated
     * elsewhere */
    m_grant_txn = new data();
    //m_grant_txn = slave_sock.get_reset_txn();
    SC_METHOD(bus_ungrant);
    sensitive << bus_ungrant_ev;
    dont_initialize();

    SC_METHOD(bus_grant);
    sensitive << bus_grant_ev;
    dont_initialize();

    SC_METHOD(pendingReqProcess);
    sensitive << end_data_ev;
    dont_initialize();

    #if AHB_SLAVE > 0
    master_sock_ahb0.bind(*this);
    #endif
    #if AHB_SLAVE > 1
    master_sock_ahb1.bind(*this);
    #endif
    #if TLM_SLAVE > 0
    master_sock_tlm0.bind(*this);
    #endif
    #if TLM_SLAVE > 1
    master_sock_tlm1.bind(*this);
    #endif
    #if MASTER_N > 0
    slave_sock0.bind(*this);
    #endif
    #if MASTER_N > 1
    slave_sock1.bind(*this);
    #endif
    #if MASTER_N > 2
    slave_sock2.bind(*this);
    #endif

    m_default_size = (BUSWIDTH + 7) / 8;
  }

  ~ahb_simple_bus()
  {
  }
};
//LCOV_EXCL_BR_STOP

#endif /*AHB_SIMPLE_BUS_H_*/
