//========================================================================================
//
// File Name    : layer.cpp
// Description  :
// Release Date : 22/10/2014
//
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version         Author          Description
//----------------------------------------------------------------------------------------
// 2014          1.0            David Aledo     ANN layer generation (memory allocation
//                                              and initialization)
//
//=======================================================================================

#include "layer.h"
#include <stddef.h> // to define NULL

/* Constructors */
layer::layer() : initialized(false),
                 DW(NULL),
                 Wprev(NULL),
                 Db(NULL),
                 bprev(NULL),
                 a(NULL),
                 s(NULL)
{  }

layer::layer(unsigned int n_in, unsigned int n_n) :
         NumIn(n_in),
         NumN(n_n),
         initialized(true)
{
   /* Dynamic memory */
   //LCOV_EXCL_BR_START
   b = new int[NumN];
   Db = new float[NumN];
   bprev = new int[NumN];
   a = new int[NumN];
   s = new float[NumN];
   W = new int*[NumN];
   DW = new float*[NumN];
   Wprev = new int*[NumN];
   //LCOV_EXCL_BR_STOP

   for(int n=0; n<NumN; n++)
   {
      //LCOV_EXCL_BR_START
      W[n] = new int[NumIn];
      DW[n] = new float[NumIn];
      Wprev[n] = new int[NumIn];
      //LCOV_EXCL_BR_STOP
   }
}

layer::~layer()
{
   //LCOV_EXCL_BR_START
   delete [] b;
   delete [] Db;
   delete [] bprev;
   delete [] a;
   delete [] s;
   //LCOV_EXCL_BR_STOP

   for(int n=0; n<NumN; n++)
   {
      //LCOV_EXCL_BR_START
      delete [] W[n];
      delete [] DW[n];
      delete [] Wprev[n];
      //LCOV_EXCL_BR_STOP
   }

   //LCOV_EXCL_BR_START
   delete [] W;
   delete [] DW;
   delete [] Wprev;
   //LCOV_EXCL_BR_STOP
}

/* Methods */
int layer::initialize(unsigned int n_in, unsigned int n_n)
{
   if(!initialized)
   {
      initialized = true;
      NumIn = n_in;
      NumN = n_n;
      //LCOV_EXCL_BR_START
      b = new int[NumN];
      Db = new float[NumN];
      bprev = new int[NumN];
      a = new int[NumN];
      s = new float[NumN];
      W = new int*[NumN];
      DW = new float*[NumN];
      Wprev = new int*[NumN];
      //LCOV_EXCL_BR_STOP

      for(int n=0; n<NumN; n++)
      {
         //LCOV_EXCL_BR_START
         W[n] = new int[NumIn];
         DW[n] = new float[NumIn];
         Wprev[n] = new int[NumIn];
         //LCOV_EXCL_BR_STOP
      }

      return 0;
   }
   else
      return -1;
}
