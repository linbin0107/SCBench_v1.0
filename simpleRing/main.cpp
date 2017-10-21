//*****************************************************************************
//
//  Simple ring example
//
//*****************************************************************************

//
// In this example there is a ring of nodes, all nodes are equivalent.
// Nodes send messages to each other. A node has unique adress.
// Each node is connected to two nodes.
// A node can transmit message from left node to right one (one-directed ring).
// If a message is adressed to the node it takes the message and remove it from ring.
//
//    left - fleft -> this -> fright - right -> ...
//    node            node             node
//                   |    |
//                  fin  fout 
//                   |    |
//                  testbench
//
// This variant symbol from ring (left node) has higer priority than symbol from tb_source,
// so no deadlock and no data losses is possible.


#include "systemc.h"
#include "node.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
int sc_main(int argc, char *argv[]) {
  sc_clock clk("clock", 10, SC_NS);

  srand(time(NULL));

  // Bind fifo for make a ring
  node_module first("n1");
  first.set_id(1);
  first.clk(clk);

  node_module sec("n2");
  sec.set_id(2);
  sec.clk(clk);

  node_module third("n3");
  third.set_id(3);
  third.clk(clk);

  first.n_buf = &(sec.in_buf);
  sec.n_buf = &(third.in_buf);
  third.n_buf = &(first.in_buf);

  sc_start(100, SC_NS);

  return 0;
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
