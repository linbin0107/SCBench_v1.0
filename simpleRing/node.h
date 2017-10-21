//
// Node module contains processes of node and processes of devices connected to node
//

#ifndef _NODE_H_
#define _NODE_H_

#include "systemc.h"
#include "buffer.h"
#include "frame.h"

// Number of nodes in the ring
static const int NODE_NUM = 3;

//LCOV_EXCL_BR_START
SC_MODULE( node_module ) {
  sc_in_clk clk;

  static const int BUF_SIZE = 30;
  static const int FRAME_LENGTH = 7; // Header + data length
  //static const int DATA_LENGTH = FRAME_LENGTH - 2; // Data length

  int id; // Node address

  // For communication with other nodes
  Buffer in_buf;        // Input buffer
  Buffer out_buf;       // Output buffer
  Buffer proc_buf;      // Processing buffer
  Buffer* n_buf;        // Input buffer of the neighbor

  // Events
  sc_event proc_frame_event;
  sc_event trans_frame_event;
  sc_event actuator_event;
  sc_event send_event;

  // Analysis of a received frame
  // This process get a frame from input buffer, analyze receiver address and
  // notify event for process which write frame into inner buffer or event
  // for process which write frame into output buffer
  void node_input_proc();

  // Copy to inner buffer
  // This process wakes up on event, copies all symbols of analysed frame
  // and removes the frame from imput buffer
  void copy_inner_proc();

  // Actuator device connected to node - uses information recieved from other nodes
  void actuator_proc();

  // Sensor device connected to node - provides information for transmit
  void sensor_proc();

  // Move frame from input buffer to putput buffer
  void trans_proc();

  // node_output_proc
  void node_output_proc();

public:
  void set_id(int id);

  // Constructor
  SC_CTOR( node_module ) : in_buf(BUF_SIZE), out_buf(BUF_SIZE), proc_buf(BUF_SIZE)
  {
    SC_THREAD(node_input_proc);
    sensitive << clk.pos();
    SC_THREAD(copy_inner_proc);
    sensitive << clk.pos();
    SC_THREAD(sensor_proc);
    sensitive << clk.pos();
    SC_THREAD(actuator_proc);
    sensitive << clk.pos();
    SC_THREAD(trans_proc);
    sensitive << clk.pos();
    SC_THREAD(node_output_proc);
    sensitive << clk.pos();
  }
};
//LCOV_EXCL_BR_STOP

#endif
