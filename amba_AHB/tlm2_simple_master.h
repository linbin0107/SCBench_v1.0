#ifndef TLM2_SIMPLE_MASTER_H_
#define TLM2_SIMPLE_MASTER_H_
#include <sstream>

#include "data.h"
#include "defines.h"

//LCOV_EXCL_BR_START
class tlm2_simple_master : public sc_core::sc_module,
                           public tlm::tlm_bw_transport_if<tlmdata>
{
public:
  tlm::tlm_initiator_socket<BUSWIDTH, tlmdata, 1> init_sock;
  sc_core::sc_in_clk clk;

  void clock_tick();
  void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
  tlm::tlm_sync_enum nb_transport_bw(tlmdata &tran, int &phase, sc_core::sc_time &t);
  void thread_process();

  sc_core::sc_event pos_clk_edge;
  tlmdata d;

  SC_HAS_PROCESS(tlm2_simple_master);

  tlm2_simple_master(sc_core::sc_module_name nm) :
                     sc_module(nm),
                     init_sock("simple_master_init_sock")
  {
    init_sock.bind(*this);

    SC_METHOD(clock_tick);
    sensitive << clk;
    dont_initialize();

    SC_THREAD(thread_process);
  }

  ~tlm2_simple_master()
  {
  }
};
//LCOV_EXCL_BR_STOP

#endif /* TLM2_MASTER_H_ */
