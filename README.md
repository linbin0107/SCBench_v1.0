# The SCBench Benchmark Suite

## Overview

The SCBench benchmark suite has been developed for SystemC verification and validation. The designs included in the suite are selected from a wide variety of applications domains. Key features of SCBench are described as follows.

1. SCBench consists of 38 well-written representive SystemC designs from various application domains, such as CPU architecutre, security, DSP, image processing, network, and telecommunication.
2. The designs range from small single process designs to large multi-process designs. All designs are selected carefully to cover as many SystemC features as possible.
3. Each design has been provided a set of stimuli and a testbench including stimuli applications and output monitors.
4. SCBench is freely available online to all researchers.

More detals of this benchmark suite can be found in our paper entitled "SCBench:
A Benchmark Design Suite for SystemC Verification and Validation".


## Compilation

1. Set your SYSTEMC_HOME environment variable.
2. You can compile all designs in the top level folder.
3. Or you can compile each design separately in its subfolder.

In addition to the design itself, each design subfolder also includes the
following files:
1. Makefile -- Includes a common makefile named Makefile.common     in the top level folder.
2. main.cpp -- Instantiates the modules which sends and receive    s data and the design.
3. tb_benchmark.cpp/.h -- Testbech module for the given design.


## Contact
If you have any questions or comments regarding to SCBench, please
contact the SCBench developers: svl@cs.pdx.edu

