*********************************************************************************
*********************************************************************************
                         The SCBench Benchmark Suite
*********************************************************************************
*********************************************************************************


The SCBench benchmark suite is intended to promote research on SystemC
verification and validation by providing 38 representative designs that
are reasonably large and well-written. Most designs are collected from open
source projects or benchmarks. More details of this benchmark suite can found in
our paper entitled "SCBench: A Benchmark Design Suite for SystemC Verification
and Validation".


Compilation
    1. Set your SYSTEMC_HOME environment variable.
    2. You can compile all designs in the top level folder.
    3. Or you can compile each design separately in its subfolder.


In addition to the design itself, each design subfolder also includes the
following files:
    1. Makefile -- Includes a common makefile named Makefile.common in the top
       level folder.
    2. main.cpp -- Instantiates the modules which sends and receives data and
       the design.
    3. tb_benchmark.cpp/.h -- Testbech module for the given design.


If you have any questions or comments regarding to SCBench, please contact the
SCBench developers: svl@cs.pdx.edu
