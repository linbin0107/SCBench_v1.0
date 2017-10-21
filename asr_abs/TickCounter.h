#include <systemc.h>
#include "settings.h"

SC_MODULE(TickCounter){

//-*-*-*-*-*-*-*-*-*-*-*-*  INTERFACE  *-*-*-*-*-*-*-*-*-*-*-*-

  sc_in<bool> inTicks; 
  sc_fifo_out<int> outTicks; 

  // counter
  int ticks;       
  
  void count(){
      ticks = ticks + 1;
  }
 
//-*-*-*-*-*-*-*-*-*-*-*-*-* SENDER *-*-*-*-*-*-*-*-*-*-*-*-*-
  
  // send ticks to ECU and reset tick counter
  void send(){
    ticks = 0;
    while(true){
        wait(TICKPERIOD, SC_MS);
 	outTicks.write(ticks); 
	ticks = 0; 
    }
  }

//-*-*-*-*-*-*-*-*-*-*-*-*-*-* CONSTRUCTOR *-*-*-*-*-*-*-*-*-*-*-*-*-*-
  
SC_CTOR(TickCounter){
    SC_METHOD(count);
    sensitive << inTicks;
    SC_THREAD(send);
  } 

}; 
