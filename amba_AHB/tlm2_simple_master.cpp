#include "tlm2_simple_master.h"


void tlm2_simple_master::clock_tick()
{
  pos_clk_edge.notify();
}

void tlm2_simple_master::thread_process()
{
  sc_time zero_delay = SC_ZERO_TIME;//LCOV_EXCL_BR_LINE
  sc_time delay;
  uint64 adr;
  unsigned int cmd = tlm::TLM_IGNORE_COMMAND;
  unsigned short burst_len;
  unsigned short number_of_slaves = SLAVE_N;
  unsigned int slave_addr_range = MEM_SIZE;
  unsigned int dataArr[BURST_LENGTH];

  while(true)
  {
    for (unsigned int i = 0; i < slave_addr_range * number_of_slaves; i = i + 3)
    {
      for (int l = 0; l < 2; l++)
      {

        burst_len = BURST_LENGTH;
        unsigned int *dptr = &dataArr[0];

        // generate random address. address range must fit to slaves.
        //adr = rand() % (number_of_slaves*slave_addr_range);

        adr = i % (number_of_slaves * slave_addr_range);

        // addr_max higher than addr_range.
        if (adr > ((number_of_slaves * slave_addr_range) - (burst_len))) {
          adr = (number_of_slaves * slave_addr_range) - (burst_len); //adr=max
        }

        // Timing annotation models processing time
        // of initiator prior to call
        delay = SMALL_DELAY;
        //delay = sc_time(rand_ps(), SC_PS);

        // generate random command.
        //tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);
        if (l == 0) {
          cmd = tlm::TLM_WRITE_COMMAND;
        } else if (l == 1) {
          cmd = tlm::TLM_READ_COMMAND;
        }

        d.data_ptr = dptr;
        d.address = adr;
        d.data_length = burst_len; //TODO: values 1,2,3,8 possible?
        d.streaming_width = burst_len;
        d.response_status = tlm::TLM_INCOMPLETE_RESPONSE;

        switch (cmd) {
        case (tlm::TLM_WRITE_COMMAND):
          d.command = tlm::TLM_WRITE_COMMAND;
          //generate random data
          for (int j = 0; j < burst_len; j++) {
            dataArr[j] = 1; //j+1;
          }
          //data[rand() & 16] = adr;

          cout << HEADER << "WRITE :: ADDR=" << d.address;//LCOV_EXCL_BR_LINE
          for (int k = 0; k < burst_len; k++) {
            cout << ", DATA[" << k << "]=" << dataArr[k];//LCOV_EXCL_BR_LINE
          }
          cout << endl;//LCOV_EXCL_BR_LINE

          break;

        case (tlm::TLM_READ_COMMAND):
          d.command = tlm::TLM_READ_COMMAND;
          break;

        default:
          d.command = tlm::TLM_IGNORE_COMMAND;
          break;
        }

        DEBUG_OUT("Sending data ...");
        init_sock->b_transport(d, delay);//LCOV_EXCL_BR_LINE

        DEBUG_OUT("Transmission finished.");

        if (cmd == tlm::TLM_READ_COMMAND) {
          cout << HEADER << "READ :: ADDR="<< d.address;//LCOV_EXCL_BR_LINE

          for (int m = 0; m < burst_len; m++) {
            cout << ", DATA[" << m << "]=" << dataArr[m];//LCOV_EXCL_BR_LINE
          }

          cout << endl;
        }

        if (d.response_status == tlm::TLM_OK_RESPONSE)
        {
          cout << HEADER << "Transaction successfully finished!" << endl;//LCOV_EXCL_BR_LINE
        }

        int wait_clks = 20;

        //LCOV_EXCL_BR_START
        cout << HEADER << " wait " << wait_clks <<
          " clk's until next transport..." << endl;
        //LCOV_EXCL_BR_STOP

        for (int n = 0; n < wait_clks; n++)
        {
          wait(pos_clk_edge);//LCOV_EXCL_BR_LINE
        }

        //LCOV_EXCL_BR_START
        cout << HEADER << " wait " << wait_clks <<
          " ...next transport will be started!" << endl;
        //LCOV_EXCL_BR_STOP
      }
    }
  }

  sc_core::sc_stop();
}

tlm::tlm_sync_enum tlm2_simple_master::nb_transport_bw(tlmdata &tran,
                                                       int &phase, sc_time &t)
{
  return tlm::TLM_COMPLETED;
}

void tlm2_simple_master::invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
                                                   sc_dt::uint64 end_range)
{

}
