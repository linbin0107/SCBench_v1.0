//========================================================================================
//
// File Name    : image.h
// Description  : hw image implemenetation
// Release Date : 10/02/2015
// Author       : PolyU DARC Lab
//                Benjamin Carrion Schafer, Xu Siyuan
//
// Revision History
//---------------------------------------------------------------------------------------
// Date        Version    Author            Description
//---------------------------------------------------------------------------------------
//10/02/2015       1.1     PolyU DARC Lab     main hw image definition header
//=======================================================================================

#include "image.h"

void image::image_main(void){
  //row that first color will persist until
  const int  pixels_y = 5;
  //column that first color will persist until
  const int  pixels_x = 5;

  wait();//LCOV_EXCL_BR_LINE

  while(1){
	if(disp_ena == true) { //--display time
   	   if (row < pixels_y && column < pixels_x) {
         //LCOV_EXCL_BR_START
         red.write(0xaa);
         green.write(0xbb);
         blue.write(0xcc);
         //LCOV_EXCL_BR_STOP
       } else {
         //LCOV_EXCL_BR_START
         red.write(0xdd);
  	     green.write(0xee);
  	     blue.write(0xff);
         //LCOV_EXCL_BR_STOP
       }
	} else {  // blanking time
      //LCOV_EXCL_BR_START
      red.write(0x11);
   	  green.write(0x22);
   	  blue.write(0x33);
      //LCOV_EXCL_BR_STOP
    }

   	wait();//LCOV_EXCL_BR_LINE
  }
}//LCOV_EXCL_BR_LINE
