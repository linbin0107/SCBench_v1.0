
#ifndef IA32_EX20_8_H
#define IA32_EX20_8_H

#include <systemc>

using namespace std;
using namespace sc_core;
using namespace sc_dt;

#define PARCEL_SIZE	8 // size of buffer
#define PAIR_SIZE	8	// each "bitpair" is a byte

#define INIT_RESERVED_OR_NOTDEFINED_FLAG false
#define INIT_ADDRESS_MODE true
#define INIT_OPERAND_MODE true

const char PreF0h[8] = {'1','1','1','1','0','0','0','0'};
const char PreF2h[8] = {'1','1','1','1','0','0','1','0'};
const char Pre66h[8] = {'0','1','1','0','0','1','1','0'};
const char Pre67h[8] = {'0','1','1','0','0','1','1','1'};

/* one opcode byte instructions */
const char instr_AAA[8] = {'0','0','1','1','0','1','1','1'};
const char instr_AAD[8] = {'1','1','0','1','0','1','0','1'};
const char instr_AAM[8] = {'1','1','0','1','0','1','0','0'};
const char instr_AAS[8] = {'0','0','1','1','1','1','1','1'};

const char instr_ADC_000100xx[8] = {'0','0','0','1','0','0','x','x'};
const char instr_ADC_100000x1[8] = {'1','0','0','0','0','0','x','1'};
const char instr_ADC_100000x0[8] = {'1','0','0','0','0','1','x','0'};
const char instr_ADC_00010101[8] = {'0','0','0','1','0','1','0','1'};
const char instr_ADC_00010100[8] = {'0','0','0','1','0','1','0','0'};

/* Opcode Extensions for one-and two-byte opcodes */


/* The IA-32 Intel Architecture Software Developer’s Manual,
   2004. Four volumes. Intel Order Numbers 253665–253668.
   Table A-4, OPCODE MAP, Intel Corporation.
*/
const char instr_opcode_extension_gr1_100000x1[8] = {'1','0','0','0','0','0','x','1'};
const char instr_opcode_extension_gr1_100000x0[8] = {'1','0','0','0','0','0','x','0'};

const char ESCAPE[8] = {'0','0','0','0','1','1','1','1'};

/* two opcode bytes instructions */
/* start with x0Fh */
const char instr_BT_10100011[8] = {'1','0','1','0','0','0','1','1'};
const char instr_BT_10111010[8] = {'1','0','1','1','1','0','1','0'};
const char instr_BSF[8] = {'1','0','1','1','1','1','0','0'};
const char instr_BSWAP[8] = {'1','1','0','0','1','x','x','x'};

//LCOV_EXCL_BR_START
class IA32_EX20_8 : public sc_module
{
public:
  // inputs
  char	parcel_[PARCEL_SIZE * PAIR_SIZE];
  char	nextparcel_[PARCEL_SIZE * PAIR_SIZE];
  char	wrapin_[PARCEL_SIZE];

  // outputs
  char	begin_[PARCEL_SIZE];
  char	end_[PARCEL_SIZE];
  char	wrapout_[PARCEL_SIZE];

  SC_CTOR(IA32_EX20_8)
  {
    SC_THREAD(run);
  }

protected:
  // internal variables
  int 	wrap_;
  char	bitpair_[PAIR_SIZE];
  bool 	address_mode_32_;
  bool  operand_mode_32_;
  bool	reserved_or_notdefined_flag_;

  void 	init();
  bool 	in_set(const char *, const char *, int);
  bool 	in_set(const char *, int);
  void  one_byte_opcode();
  void  two_byte_opcode();
  bool 	handle_prefix();
  void 	read_one_PAIR();
  void 	length_decoder();
  void 	mod_reg_rm();
  void 	bit16_addressing_ModRM();
  void 	bit32_addressing_ModRM();
  void 	error_flag_decoder();

  void 	show();
  void 	run();
};
//LCOV_EXCL_BR_STOP

#endif
