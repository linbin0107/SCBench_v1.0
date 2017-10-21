//========================================================================================
//
// File Name    : ann.cpp
// Description  : ANN main file
// Release Date : 22/10/2014
//
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version         Author                      Description
//----------------------------------------------------------------------------------------
// 2014          1.0            David Aledo, UPM             Artificial Neural Network
//
//=======================================================================================

#include "ann.h"

//LCOV_EXCL_BR_START
ann::ann(sc_module_name nm) : sc_module(nm)
{
  SC_CTHREAD(Wyb_cthread, clk.pos());
  reset_signal_is(rst,true);

  SC_CTHREAD(ann_cthread, clk.pos());
  reset_signal_is(rst,true);
}
//LCOV_EXCL_BR_STOP

void ann::Wyb_cthread()
{
   /* Memories do not need initialization */
   wait();//LCOV_EXCL_BR_LINE

   for(;;)
   {
      if(wr->read())
      {
#ifdef NLAYER_4
         write_Wyb< type_0 >(layer_0, 0);
         write_Wyb< type_1 >(layer_1, 1);
         write_Wyb< type_2 >(layer_2, 2);
         write_Wyb< type_3 >(layer_3, 3);
#else
         write_Wyb< type_0 >(layer_0, 0);
         write_Wyb< type_1 >(layer_1, 1);
#endif
      }
      else
         wait();//LCOV_EXCL_BR_LINE
   }
}

template <class Lt>
void ann::write_Wyb(Lt &Wyb, int l){

   /* Write weights first */
   for(int n=0; n<NumN[l+1]; n++)
   {
      for(int i=0; i<NumN[l]; i++)
      {
         Wyb.W[n][i] = wdata->read();//LCOV_EXCL_BR_LINE
         wait();//LCOV_EXCL_BR_LINE
      }
   }
   /* Write biases second */
   for(int n=0; n<NumN[l+1]; n++)
   {
      Wyb.b[n] = wdata->read();//LCOV_EXCL_BR_LINE
      wait();//LCOV_EXCL_BR_LINE
   }
}


/***************************
 **
 ** Main ANN SC_CThread
 **
 ***************************/
void ann::ann_cthread()
{
   wait();//LCOV_EXCL_BR_LINE

   for(;;)
   {
      run_out->write(false);//LCOV_EXCL_BR_LINE
      if (run_in->read())
      {
         /* 1.)  Read inputs to buffer */
         for(int i=0; i<NumN[0]; i++)
         {
            buff[0][i] = inputs[i]->read();//LCOV_EXCL_BR_LINE
         }

         /* 2.) Calls to the layer function: */
#ifdef NLAYER_4
	layer< type_0 >(layer_0, 0);
	layer< type_1 >(layer_1, 1);
	layer< type_2 >(layer_2, 2);
	layer< type_3 >(layer_3, 3);
#else
	layer< type_0 >(layer_0, 0);
	layer< type_1 >(layer_1, 1);
#endif

	 /* 3.) Write outputs */

#ifdef TRAIN
         for(int l=0; l<(Nlayer-1); l++)
         {
            for(int i=0; i<NumN[l+1]; i++)
               a[l][i] = buff[l+1][i];//LCOV_EXCL_BR_LINE
         }
#endif

         for(int i=0; i<NumN[Nlayer]; i++)
         {
            int clipp = buff[Nlayer][i];//LCOV_EXCL_BR_LINE
            //Clipping
            if (clipp >= 1<<(NbitOut-1))
               //0x00007FFF for 16 bits
               outputs[i]->write((1<<(NbitOut-1))-1); //LCOV_EXCL_BR_LINE
            else if (clipp <= -1<<(NbitOut-1))
               //0xFFFF8000 for 16 bits
               outputs[i]->write(-1<<(NbitOut-1)); //LCOV_EXCL_BR_LINE
            else
               outputs[i]->write(clipp);//LCOV_EXCL_BR_LINE

         }

         run_out->write(true);//LCOV_EXCL_BR_LINE
      }
      wait();//LCOV_EXCL_BR_LINE
   }
}

template <class Lt>
void ann::layer(Lt &Wyb, int l)
{
  //Important: Order of nested loops for inputs and neurons
  for(int n=0; n<NumN[l+1]; n++)
  {
     //bias
     int acc = Wyb.b[n];//LCOV_EXCL_BR_LINE
     for(int i=0; i<NumN[l]; i++)
     {
        acc += buff[l][i]*Wyb.W[n][i];//LCOV_EXCL_BR_LINE
     }
     buff[l+1][n] = acc>>trunc_out;//LCOV_EXCL_BR_LINE
  }
}//LCOV_EXCL_BR_LINE
