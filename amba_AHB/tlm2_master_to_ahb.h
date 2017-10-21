#ifndef CONV_TLM2_TO_AHB_H_
#define CONV_TLM2_TO_AHB_H_

#include "data.h"
#include "defines.h"
#include <tlm_utils/peq_with_cb_and_phase.h>

//LCOV_EXCL_BR_START
class tlm2_master_to_ahb : public sc_core::sc_module,
                           public tlm::tlm_bw_transport_if<data>,
                           public tlm::tlm_fw_transport_if<tlmdata>
{
public:
  tlm::tlm_initiator_socket<BUSWIDTH, data, 1> master_socket;

  /* @brief slave socket to connect to tlm2 master. */
  tlm::tlm_target_socket<BUSWIDTH, tlmdata, 1> slave_socket;

  sc_core::sc_in_clk clk;

  void busGranted();
  void busNotGranted();
  void b_transport(tlmdata &tran, sc_core::sc_time &t);
  void clock_tick();
  void dataProcess();
  bool get_direct_mem_ptr(tlmdata &trans, tlm::tlm_dmi &dmi_data);
  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
  tlm::tlm_sync_enum nb_transport_bw(data &tran, int &phase, sc_core::sc_time &t);
  tlm::tlm_sync_enum nb_transport_fw(tlmdata &tran, int &phase, sc_core::sc_time &t);
  void peq_cb(data &tran, const int &phase);
  void receiveData(data &tran);
  void resumeRequest();
  void requestProcess();
  void sendData();
  void startRequest();
  unsigned int transport_dbg(tlmdata &trans);

  sc_core::sc_event b_transport_finished_ev;
  sc_dt::uint64 current_Address;
  unsigned int current_Command;
  unsigned int current_DataLength;
  data *current_trans;
  unsigned int master_id;
  bool m_bus_granted;
  bool m_bus_req_pending;
  unsigned int m_data_count;
  bool m_data_pending;
  tlm_utils::peq_with_cb_and_phase< tlm2_master_to_ahb, data > m_peq;
  bool m_request_pending;
  bool m_restart_pending_req;
  unsigned int m_txn_count;

  SC_HAS_PROCESS(tlm2_master_to_ahb);

  tlm2_master_to_ahb(sc_core::sc_module_name nm, unsigned int id) : sc_module(nm)
  ,master_socket("tlm2_master_to_ahb_socket")
  ,slave_socket("bus_socket")
  ,current_DataLength(0)
  ,current_trans(NULL)
  ,master_id(id)
  ,m_bus_granted(false)
  ,m_bus_req_pending(false)
  ,m_data_count(0)
  ,m_data_pending(false)
  ,m_peq("m_peq", this, &tlm2_master_to_ahb::peq_cb)
  ,m_request_pending(false)
  ,m_restart_pending_req(false)
  ,m_txn_count(0)
  {
    SC_METHOD(clock_tick);
    sensitive << clk;
    dont_initialize();

    master_socket.bind( *this );
    slave_socket.bind( *this );
  }

  ~tlm2_master_to_ahb()
  {
  }

};
//LCOV_EXCL_BR_STOP

#endif /*CONV_TLM2_TO_AHB_H_*/

