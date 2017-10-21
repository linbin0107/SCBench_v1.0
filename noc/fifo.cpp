#include <systemc>
#include <iostream>
#include "fifo.h"
using namespace sc_core;
using namespace std;

void virtual_fifo::receive_fifo(){
    sc_uint<2> x_to_fifo;//LCOV_EXCL_BR_LINE
    bool data_in_tmp;
    int fifo_num_free, fifo1_num_free, fifo2_num_free;

	while(true){
        //LCOV_EXCL_BR_START
        x_to_fifo = x_to_fifo_sel.read();
        fifo_num_free= fifo.num_free();
        fifo1_num_free= fifo1.num_free();
        fifo2_num_free= fifo2.num_free();
        data_in_tmp = data_in.read();
        //LCOV_EXCL_BR_STOP
		if(x_to_fifo == 1){			//select fifo.
			if(fifo_num_free > 0){
				fifo.write(data_in_tmp);//LCOV_EXCL_BR_LINE
			}
		}
		else if(x_to_fifo == 2){		//select fifo1.
			if(fifo1_num_free > 0){
				fifo1.write(data_in_tmp);//LCOV_EXCL_BR_LINE
			}
		}
		else if(x_to_fifo == 3){		//select fifo2.
			if(fifo2_num_free > 0){
				fifo2.write(data_in_tmp);//LCOV_EXCL_BR_LINE
			}
		}
#ifdef FIFO_DEBUG
		if(fifo.num_free() == 0){
			fifo.print(cout);
			cout << endl;
		}
#endif
		wait();//LCOV_EXCL_BR_LINE
	}
}

void virtual_fifo::transfer_fifo(){
    sc_uint<2> fifo_to_x;//LCOV_EXCL_BR_LINE
	bool temp;
    int fifo_num_free, fifo1_num_free, fifo2_num_free;

	while(true){
        //LCOV_EXCL_BR_START
        fifo_to_x = fifo_to_x_sel.read();
        fifo_num_free= fifo.num_free();
        fifo1_num_free= fifo1.num_free();
        fifo2_num_free= fifo2.num_free();
        //LCOV_EXCL_BR_STOP
		if(fifo_to_x == 1){			//select fifo.
			if(fifo_num_free < depth){
				temp = fifo.read();//LCOV_EXCL_BR_LINE
				data_out.write(temp);//LCOV_EXCL_BR_LINE
			}
		}
		else if(fifo_to_x == 2){		//select fifo1.
			if(fifo1_num_free < depth){
				temp = fifo1.read();//LCOV_EXCL_BR_LINE
				data_out.write(temp);//LCOV_EXCL_BR_LINE
			}
		}
		else if(fifo_to_x == 3){		//select fifo1.
			if(fifo2_num_free < depth){
				temp = fifo2.read();//LCOV_EXCL_BR_LINE
				data_out.write(temp);//LCOV_EXCL_BR_LINE
			}
		}
		wait();//LCOV_EXCL_BR_LINE
	}
}

void virtual_fifo :: full_signal(){
	sc_uint<3>	full_state = 0;//LCOV_EXCL_BR_LINE
    int fifo_num_free, fifo1_num_free, fifo2_num_free;

	full.write(0x00);//LCOV_EXCL_BR_LINE

	while(true){
		//full signal.
        //LCOV_EXCL_BR_START
        fifo_num_free= fifo.num_free();
        fifo1_num_free= fifo1.num_free();
        fifo2_num_free= fifo2.num_free();
        //LCOV_EXCL_BR_STOP
		if(fifo_num_free == 0){
			full_state |= 0x01;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo_num_free > 0){
			full_state &= ~0x01;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
		if(fifo1_num_free == 0){
			full_state |= 0x02;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo1_num_free > 0){
			full_state &= ~0x02;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
		if(fifo2_num_free == 0){
			full_state |= 0x04;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo2_num_free > 0){
			full_state &= ~0x04;
			full.write(full_state);//LCOV_EXCL_BR_LINE
		}
#ifdef FIFO_DEBUG
//		cout << "full signal: " << full_state << endl;
#endif
		//wait(1,SC_NS);//LCOV_EXCL_BR_LINE
		wait();//LCOV_EXCL_BR_LINE
	}
}

void virtual_fifo :: empty_signal(){
	sc_uint<3> empty_state = 7;//LCOV_EXCL_BR_LINE
    int fifo_num_free, fifo1_num_free, fifo2_num_free;

	empty.write(0x07);//LCOV_EXCL_BR_LINE

	while(true){
		//empty signal.
        //LCOV_EXCL_BR_START
        fifo_num_free = fifo.num_free();
        fifo1_num_free= fifo1.num_free();
        fifo2_num_free= fifo2.num_free();
        //LCOV_EXCL_BR_STOP
		if(fifo_num_free == depth){
			empty_state |= 0x01;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo_num_free < depth){
			empty_state &= ~0x01;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
		if(fifo1_num_free == depth){
			empty_state |= 0x02;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo1_num_free < depth){
			empty_state &= ~0x02;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
		if(fifo2_num_free == depth){
			empty_state |= 0x04;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
		else if(fifo2_num_free < depth){
			empty_state &= ~0x04;
			empty.write(empty_state);//LCOV_EXCL_BR_LINE
		}
#ifdef FIFO_DEBUG
//		cout << "empty signal: " << empty_state << endl;
#endif
		//wait(1,SC_NS);//LCOV_EXCL_BR_LINE
		wait();//LCOV_EXCL_BR_LINE
	}
}//LCOV_EXCL_BR_LINE
