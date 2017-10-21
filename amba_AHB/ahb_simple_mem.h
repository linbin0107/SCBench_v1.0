#ifndef AHB_SIMPLE_SLAVE_H_
#define AHB_SIMPLE_SLAVE_H_

#include "data.h"
#include "defines.h"
#include <tlm_utils/peq_with_cb_and_phase.h>

//LCOV_EXCL_BR_START
class ahb_simple_mem : public sc_core::sc_module, tlm::tlm_fw_transport_if<data>
{
public:
  tlm::tlm_target_socket<BUSWIDTH, data, 1> slave_socket;
  sc_core::sc_in_clk clk;

  void b_transport(data &tran, sc_core::sc_time &t );
  bool get_direct_mem_ptr(data &tran, tlm::tlm_dmi &dmi_data);
  void invalidate_direct_mem_ptr(int start_range, int end_range);
  tlm::tlm_sync_enum nb_transport_fw(data &tran, int &phase, sc_core::sc_time &t);
  void peq_cb(data &tran, const int &phase);
  void readData(unsigned int *burst_size);
  void responseProcess();
  unsigned int transport_dbg(data& tran);
  void writeData(unsigned int *burst_size);

  unsigned int baseAddr; //base address of the slave
  unsigned int counter;
  data *current_trans;
  unsigned int MEM[MEM_SIZE];
  unsigned int mem_size; //size of the memory
  bool m_data_pending;
  tlm_utils::peq_with_cb_and_phase< ahb_simple_mem, data > m_peq;
  bool m_request_pending;
  unsigned int txn_address;
  unsigned int txn_burst_len;

  SC_HAS_PROCESS(ahb_simple_mem);

  ahb_simple_mem(sc_module_name nm, unsigned int addr, unsigned int tsize) : sc_module(nm)
  ,slave_socket("ahb_simple_slave_socket")
  ,baseAddr(addr)
  ,counter(0)
  ,current_trans(NULL)
  ,mem_size(tsize)
  ,m_data_pending(false)
  ,m_peq("slave peq", this, &ahb_simple_mem::peq_cb)
  ,m_request_pending(false)
  ,txn_address(0)
  ,txn_burst_len(0)
  {
    for(unsigned int i = 0; i < mem_size; i++) {
      MEM[i] = 0;
    }

    SC_METHOD(responseProcess);
    sensitive << clk;
    dont_initialize();

    slave_socket.bind( *this );
  }

  ~ahb_simple_mem()
    {
      //  delete[] MEM;
  }

  inline unsigned int get_size()
  {
    return mem_size;
  }

  inline unsigned int get_base_addr()
  {
    return baseAddr;
  }

};
//LCOV_EXCL_BR_STOP

#endif /*TLM2_SIMPLE_SLAVE_AHB_H_*/
