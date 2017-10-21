#ifndef DEFINES_H_
#define DEFINES_H_

#define MEM_SIZE 3
#define AHB_SLAVE 2
#define TLM_SLAVE 0
#define SLAVE_N AHB_SLAVE+TLM_SLAVE
#define MASTER_N 2

//#define DEBUG
#define HEADER sc_time_stamp() << " /" << (unsigned) sc_core::sc_delta_count() << " " << name() << ": "

#define BUSWIDTH 32
#define BURST_SIZE 1 //BUSWIDTH/8
#define BURST_LENGTH MEM_SIZE

#define BEGIN_DATA 42
#define END_DATA 43
#define DATA_RETRY 44

#define NON_SEQUENTIAL 50
#define SEQUENTIAL 51
#define IDLE 52
#define BUSY 53

#define BUS_REQ 60
#define BUS_GRANT 61
#define BUS_UNGRANT 62
#define BUS_REQ_STOP 63

#define SC_ZERO_TIME sc_time(0, SC_NS)
//this is the delay all PEQs do realize. In the original model this is 1 PS but no process interferes with it so we can extend the period to 1 NS in order to have a unified time resolution. 
#define SMALL_DELAY sc_time(1, SC_NS)

#ifdef DEBUG
	#define DEBUG_OUT(TXT); cout << HEADER << TXT << endl;
#else
	#define DEBUG_OUT(TXT);
#endif

#define PHASE_OUT(PH) (PH == BEGIN_DATA ? "BEGIN_DATA" : (PH == END_DATA ? "END_DATA" : (PH == DATA_RETRY ? "DATA_RETRY" : (PH == tlm::BEGIN_REQ ? "BEGIN_REQ" : (PH == tlm::END_REQ ? "tlm::END_REQ" : (PH == tlm::BEGIN_RESP ? "tlm::BEGIN_RESP" : (PH == tlm::END_RESP ? "tlm::END_RESP" : (PH == BUS_REQ ? "BUS_REQ" : (PH == BUS_GRANT ? "BUS_GRANT" : (PH == BUS_UNGRANT ? "BUS_UNGRANT" : (PH == BUS_REQ_STOP ? "BUS_REQ_STOP" : "UNKNOWN")))))))))))

#endif
