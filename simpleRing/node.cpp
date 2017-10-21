//
// Node module contains processes of node and processes of devices connected to node
//

#include "node.h"

//=====================================================================================
// Analysis of a received frame
// This process get a frame from input buffer, analyze receiver address and
// notify event for process which write frame into inner buffer or event
// for process which write frame into output buffer
void node_module::node_input_proc() {
  wait();//LCOV_EXCL_BR_LINE

  while(true) {
    int a[FRAME_LENGTH];
    // Try to get enough symbols
    if (in_buf.get(FRAME_LENGTH, a)) {
      // Check if there is a symbol in input buffer
      Frame f(a, FRAME_LENGTH);//LCOV_EXCL_BR_LINE
      if (f.receiver == id) {
        // Notify copy to inner buffer process
        proc_frame_event.notify();//LCOV_EXCL_BR_LINE
      } else {
        // Notify retranslate process
        trans_frame_event.notify();//LCOV_EXCL_BR_LINE
      }
      //printf("ip : frame received from %d to %d in %d\n", f.sender, f.receiver, id);
    }

    wait();//LCOV_EXCL_BR_LINE
  }
}

//=====================================================================================
// Copy to inner buffer
// This process wakes up on event, copies all symbols of analysed frame
// and removes the frame from imput buffer
void node_module::copy_inner_proc() {
  while(true) {
    wait(proc_frame_event);//LCOV_EXCL_BR_LINE

    int a[FRAME_LENGTH];
    in_buf.get(FRAME_LENGTH, a);//LCOV_EXCL_BR_LINE
    in_buf.remove(FRAME_LENGTH);//LCOV_EXCL_BR_LINE

    // Ignore result of add call for simplification
    proc_buf.add(FRAME_LENGTH, a);//LCOV_EXCL_BR_LINE

    // Wake up the actuator
    actuator_event.notify();//LCOV_EXCL_BR_LINE
  }
}

//=====================================================================================
// Actuator device connected to node - uses information recieved from other nodes
void node_module::actuator_proc() {
  while(true) {
    wait(actuator_event);//LCOV_EXCL_BR_LINE

    int a[FRAME_LENGTH];
    if (proc_buf.get(FRAME_LENGTH, a)) {
      //LCOV_EXCL_BR_START
      proc_buf.remove(FRAME_LENGTH);
      printf("ap : frame received from %d in %d\n", a[0], a[1]);
      //LCOV_EXCL_BR_STOP
    }
  }
}

//=====================================================================================
// Sensor device connected to node - provides information for transmit
void node_module::sensor_proc() {
  while (true) {
    //for (int i = 0; i < 1; i++) {
    if (rand() % 10 > 7) {
      int address = (rand() % NODE_NUM)  + 1;
      if (address == id) {
         address = (address + 1) % NODE_NUM + 1;
      }

      //LCOV_EXCL_BR_START
      printf("sp : frame created from %d to %d\n", id, address);

      int a[FRAME_LENGTH];
      Frame f(id, address, FRAME_LENGTH);
      f.serialize(a);
      out_buf.add(FRAME_LENGTH, a);

      send_event.notify();
      //LCOV_EXCL_BR_STOP
    }
    //}

    wait();//LCOV_EXCL_BR_LINE
  }
}

//=====================================================================================
// Move frame from input buffer to putput buffer
void node_module::trans_proc() {
  while(true) {
    wait(trans_frame_event);//LCOV_EXCL_BR_LINE

    int a[FRAME_LENGTH];
    //LCOV_EXCL_BR_START
    in_buf.get(FRAME_LENGTH, a);
    in_buf.remove(FRAME_LENGTH);

    // Ignore result of add call for simplification
    out_buf.add(FRAME_LENGTH, a);
    send_event.notify();
    //LCOV_EXCL_BR_STOP
  }
}

//=====================================================================================
// node_output_proc
void node_module::node_output_proc() {
  while(true) {
    wait(send_event);//LCOV_EXCL_BR_LINE

    int a[FRAME_LENGTH];

    // Copy all frames from output buffer
    while (out_buf.get(FRAME_LENGTH, a)) {
      // printf("op : frame is sent from %d to %d\n", a[0], a[1]);
      out_buf.remove(FRAME_LENGTH);//LCOV_EXCL_BR_LINE
      n_buf->add(FRAME_LENGTH, a);//LCOV_EXCL_BR_LINE
    }
  }
}

//=====================================================================================
void node_module::set_id(int id) {
  this->id = id;
}
