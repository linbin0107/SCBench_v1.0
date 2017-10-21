#ifndef FIFO_H
#define FIFO_H

#include <systemc>
#include "define.h"

using namespace sc_core;
using namespace sc_dt;

//LCOV_EXCL_BR_START
class virtual_fifo : public sc_module{
public :
	sc_in<bool>		data_in;				//Input data port.
	sc_out<bool>	data_out;				//Output data port.

	sc_in<bool>			write_n;			//Output data control pin a.
	sc_in<bool>			read_n;				//Input data control pin.
	sc_in<sc_uint<2> >	x_to_fifo_sel;		//Fifo select signal when receive data.
	sc_in<sc_uint<2> >	fifo_to_x_sel;		//Fifo select signal when transfer data.

	sc_out<sc_uint<3> >	empty;				//State of FIFO is full.
	sc_out<sc_uint<3> >	full;				//State of FIFO is empty.

	sc_fifo<bool>	fifo, fifo1, fifo2;
    sc_uint<8> depth;

    SC_HAS_PROCESS(virtual_fifo);

	//SC_CTOR(virtual_fifo):fifo(FIFO_DEEP), fifo1(FIFO_DEEP), fifo2(FIFO_DEEP){
	virtual_fifo(sc_module_name nm, sc_uint<8> fifo_depth):
        sc_module(nm),
        fifo(fifo_depth),
        fifo1(fifo_depth),
        fifo2(fifo_depth),
        depth(fifo_depth){

		SC_THREAD(receive_fifo);
		sensitive << write_n.neg();

		SC_THREAD(transfer_fifo);
		sensitive << read_n.neg();

		SC_THREAD(full_signal);

		SC_THREAD(empty_signal);
	}
protected:
	sc_event full_alram;
	sc_event empty_alram;
	void receive_fifo();
	void transfer_fifo();
	void empty_signal();
	void full_signal();
};
//LCOV_EXCL_BR_STOP

#endif
