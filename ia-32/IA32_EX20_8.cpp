#include <iostream>
#include <systemc>

#include "IA32_EX20_8.h"

void IA32_EX20_8::show()
{
  std::cout << "begin: \t";//LCOV_EXCL_BR_LINE
  for (int i = 0; i < PARCEL_SIZE; i++)
    std::cout << begin_[i] + '0' << " ";//LCOV_EXCL_BR_LINE
  std::cout << std::endl;//LCOV_EXCL_BR_LINE

  std::cout << "end: \t";//LCOV_EXCL_BR_LINE
  for (int i = 0; i < PARCEL_SIZE; i++)
    std::cout << end_[i] + '0' << " ";//LCOV_EXCL_BR_LINE
  std::cout << std::endl;//LCOV_EXCL_BR_LINE

  std::cout << "wrapout: \t";//LCOV_EXCL_BR_LINE
  for (int i = 0; i < PARCEL_SIZE; i++)
    std::cout << wrapout_[i] + '0' << " ";//LCOV_EXCL_BR_LINE
  std::cout << std::endl;//LCOV_EXCL_BR_LINE
}

void IA32_EX20_8::init()
{}

void IA32_EX20_8::error_flag_decoder()
{
  reserved_or_notdefined_flag_ = true;
}

void IA32_EX20_8::two_byte_opcode()
{
  wrap_++;

  if(in_set(instr_BSWAP, 8) == true)
  {
    if(wrap_ - 1 < PARCEL_SIZE)
      end_[wrap_ - 1] = 1;
  }
  else
  {
    error_flag_decoder();
  }
}

void IA32_EX20_8::one_byte_opcode()
{
  wrap_++;

  if(in_set(instr_AAA, 8) == true)
  {
    if(wrap_ - 1 < PARCEL_SIZE)
      end_[wrap_ -1] = 1;
  }
  // w = 1
  else if(in_set(instr_ADC_00010101, 8) == true)
  {
    if(operand_mode_32_ == true)
      wrap_ += 4;
    else
      wrap_ += 2;

    if(wrap_ - 1 < PARCEL_SIZE)
      end_[wrap_ - 1] = 1;
  }
  //w = 0
  else if(in_set(instr_ADC_00010100, 8) == true)
  {
    wrap_ += 1;

    if(wrap_ - 1 < PARCEL_SIZE)
      end_[wrap_ - 1] = 1;
  }
  else
  {
    error_flag_decoder();
  }
}

// Function: Return TRUE if the subset and superset are same in 3-value logic
bool IA32_EX20_8::in_set(const char *subset, const char *superset, int s)
{
  for(int i = 0; i < s; i++)
  {
    if((subset[i] != superset[i]) && (superset[i] != 'x') && (superset[i] != 'X'))
      return false;
  }
  return true;
}

// Function: Return TRUE if the subset and superset are same in 3-value logic
bool IA32_EX20_8::in_set(const char *superset, int s)
{
  for(int i = 0; i < s; i++)
  {
    if((bitpair_[i] != superset[i]) && (superset[i] != 'x') && (superset[i] != 'X'))
      return false;
  }
  return true;
}

void IA32_EX20_8::read_one_PAIR()
{
  if(wrap_ < PARCEL_SIZE)
  {
    for(int i = 0; i < PAIR_SIZE; i++)
      bitpair_[i] = parcel_[wrap_ * PAIR_SIZE + i];
  }
  else
  {
    for(int i = 0; i < PAIR_SIZE; i++)
      bitpair_[i] = nextparcel_[(wrap_ - PARCEL_SIZE) * PAIR_SIZE + i];
  }
}
// Currently only handle prefix 0x66 and 0x67
// Function: only the first 0x66 or 0x67 are considered as prefix code. (currently)
// 	if there are more than one 0x66 or 0x67, raise error flag, but still consider them as fresh data
//	and read again. Is this right?
bool IA32_EX20_8::handle_prefix()
{
  char pref_gr1, pref_gr2;
  pref_gr1 = false;
  pref_gr2 = false;

  //-nan
  int wrap_prev;
  wrap_prev = -1;

  for(int i = 0; i < 4; i++)
  {
    if((wrap_prev == -1) || (wrap_prev < wrap_))
      read_one_PAIR();
    else
      break;

    wrap_prev = wrap_;

    if(strcmp(bitpair_, Pre66h) == 0)
    {
      // toggle with operand mode prefix only once
      if(pref_gr1 == false)
      {
        operand_mode_32_ = !(operand_mode_32_);
        wrap_++;
        pref_gr1 = true;
      }
      // error, only one prefix in each group
      else
      {
        error_flag_decoder();
      }

      continue;
    }

    if(strcmp(bitpair_, Pre67h) == 0)
    {
      // toggle with addressing mode prefix only once
      if(pref_gr2 == false)
      {
        address_mode_32_ = !(address_mode_32_);
        wrap_++;
        pref_gr2 = true;
      }
      // error, only one prefix in each group
      else
      {
        error_flag_decoder();
      }

      continue;
    }

    // Byte is not a prefix, this bitpair has fresh data
    if(!(pref_gr1 || pref_gr2))
      return false;
  }

  // Byte is a prefix, need to read another bitpair
  return true;
}

void IA32_EX20_8::length_decoder()
{
  // local variables
  int i;
  char ret;

  for(i = 0; i < PARCEL_SIZE; i++)
  {
    begin_[i] = 0;
    end_[i] = 0;
  }
  wrap_ = 0;

  //stage 1 Skip over previously handled instructions
  while(wrapin_[wrap_] == 0)	wrap_++;

  // end wrapped instruction
  if(wrap_ > 0)
    end_[wrap_ - 1] = 1;

  //stage 2 Start decoding instructions
  while(wrap_ < PARCEL_SIZE)
  {
    begin_[wrap_] = 1;

    operand_mode_32_ = INIT_OPERAND_MODE;
    address_mode_32_ = INIT_ADDRESS_MODE;

    ret = handle_prefix();

    if(ret == true)
      read_one_PAIR();

    if(in_set(ESCAPE, 8) == false)
      one_byte_opcode();

    if(in_set(ESCAPE, 8) == true)
    {
      read_one_PAIR();
      two_byte_opcode();
    }
  }
  wrapout_[wrap_ - PARCEL_SIZE] = 1;
}

void IA32_EX20_8::run()
{
  length_decoder();
  show();
}

void IA32_EX20_8::bit32_addressing_ModRM()
{
  // Mod == 00
  if(in_set("00", 2) == true)
  {
    if(in_set(bitpair_ + 5, "0", 1) == true) {}
    else if(in_set(bitpair_ + 5, "11", 2) == true) {}
    else if(in_set(bitpair_ + 5, "101", 3) == true)
    {
      wrap_ += 4;
    }
    else if(in_set(bitpair_ + 5, "100", 3) == true)
    {
      read_one_PAIR();
      wrap_++;
      if(in_set(bitpair_ + 2, "100", 3) == false)
      {
	if(in_set(bitpair_ + 5, "101", 3) == true)
	{
	  wrap_ += 4;
	}
      }
      else
	error_flag_decoder();
    }
  }
  // Mod == 01
  else if(in_set("01", 2) == true)
  {
    if(in_set(bitpair_ + 5, "100", 3) == false)
    {
      wrap_++;
    }
    else
    {
      read_one_PAIR();
      wrap_++;
      if(in_set(bitpair_ + 2, "100", 3) == false)
      {
        wrap_++;
      }
      else
      	error_flag_decoder();
    }
  }
  // Mod == 01
  else if(in_set("10", 2) == true)
  {
    if(in_set(bitpair_ + 5, "100", 3) == false)
    {
      wrap_ += 4;
    }
    else
    {
      read_one_PAIR();
      wrap_++;
      if(in_set(bitpair_ + 2, "100", 3) == false)
      {
        wrap_ += 4;
      }
      else
      	error_flag_decoder();
    }
  }
}


void IA32_EX20_8::bit16_addressing_ModRM()
{
  // nan
  // Mod = 00
  if(in_set("00", 2) == true)
  {
    if(in_set(bitpair_ + 5, "110", 3) == true)
      wrap_ += 2;
  }
  // Mod = 01
  else if(in_set("01", 2) == true)
  {
    wrap_ ++;
  }
  // Mod = 10
  else if(in_set("10", 2) == true)
  {
    wrap_ += 2;
  }
}

void IA32_EX20_8::mod_reg_rm()
{
  wrap_++;

  switch(address_mode_32_)
  {
    case true:
      bit32_addressing_ModRM();
      break;
    case false:
      bit16_addressing_ModRM();
      break;
  }
}
