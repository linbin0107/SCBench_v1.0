/***************************************************
 * File: tb_control.cpp
 * Description: testbench file for control unit
 * Author: Bin Lin
 * Affiliation: SVL Lab, Portland State University
 * Date: 04/01/2015
 **************************************************/

#include <cstdlib>
#include <stdint.h>
#include "tb_control.h"

//LCOV_EXCL_START
//LCOV_EXCL_BR_START
void tb_control::driver() {

  instruction_valid_out.write(false);
  alu_dout.write(false);
  dram_dout.write(false);
  fpu_dout.write(false);
  mmx_dout.write(false);
  dram_rd_valid_out.write(false);

  wait(2);
  while(1) {

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0xf1e01000);
    alu_valid_out.write(true);
    alu_dout.write(0xdeadbeef);
    wait();

    instruction_valid_out.write(false);
    wait(2);

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0x13010009);
    wait();

    instruction_valid_out.write(false);
    wait(2);

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0x05ee0001);
    wait();

    instruction_valid_out.write(false);
    wait(2);

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0x02220001);
    wait();

    instruction_valid_out.write(false);
    wait(2);

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0x1000fff8);
    wait();

    instruction_valid_out.write(false);
    wait(2);

    while (next_pc.read() == false)
      wait();

    instruction_valid_out.write(true);
    instruction_out.write(0x4d0e0001);
    wait();

    instruction_valid_out.write(false);
    wait(2);
  }
}
//LCOV_EXCL_BR_STOP
//LCOV_EXCL_STOP
