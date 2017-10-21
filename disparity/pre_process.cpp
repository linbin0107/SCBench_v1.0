//============================================================================
//
//
// File Name    : pre_process.cpp
// Description  : Main function which converts RGB to YUV
// Release Date : 24/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date        Version    Author    Description
//---------------------------------------------------------------------------
//24/07/2013     1.0                 RGB to YUV conversion and parameter compuation
//============================================================================

#include "pre_process.h"
#include "define.h"


/*
** RGB to YUV conversion
*/
void pre_process::RGBtoYUV()
{
  // Variables declaration
  // Coefficients converted to integers (ref.:http://en.wikipedia.org/wiki/YUV)
  int rgb2yuv_coeff[3][3] = {{299, 587, 114},
			     {-147, -289, 436},
			     {615, -515, -100}};

 int rgb2yuv_offset[3] = {18, 132, 132};

 int Left_R;
 int Left_G;
 int Left_B;

 int Right_R;
 int Right_G;
 int Right_B;

 int Left_tmp;
 int Right_tmp;

  bool valid_in_tmp;

  //Reset
  //LCOV_EXCL_BR_START
  VSynch_load.write(true);
  HSynch_load.write(true);
  Height_load.write(0);
  Width_load.write(0);
  Left_pixel_Y_load.write(0);
  Right_pixel_Y_load.write(0);
  valid_in_load.write(false);

  wait();
  //LCOV_EXCL_BR_STOP
  while(1){
    //LCOV_EXCL_BR_START
    Left_R=Left_pixel_R.read();
    Left_B=Left_pixel_B.read();
    Left_G=Left_pixel_G.read();

    Right_R=Right_pixel_R.read();
    Right_G=Right_pixel_G.read();
    Right_B=Right_pixel_B.read();
    //LCOV_EXCL_BR_STOP

    Left_tmp = (int)(( rgb2yuv_coeff[0][0]*(int)(Left_B)
			+ rgb2yuv_coeff[0][1]*(int)(Left_G)
			+ rgb2yuv_coeff[0][2]*(int)(Left_R))/1024
		        + rgb2yuv_offset[0]);

    Right_tmp = (int)(( rgb2yuv_coeff[0][0]*(int)(Right_B)
			+ rgb2yuv_coeff[0][1]*(int)(Right_G)
			+ rgb2yuv_coeff[0][2]*(int)(Right_R))/1024
		        + rgb2yuv_offset[0]);

    valid_in_tmp = valid_in.read();//LCOV_EXCL_BR_LINE
    if(valid_in_tmp == true){
      //LCOV_EXCL_BR_START
      Left_pixel_Y_load.write(Left_tmp);
      Right_pixel_Y_load.write(Right_tmp);
      valid_in_load.write(true);
      //LCOV_EXCL_BR_STROP
    }
    else{
      valid_in_load.write(false);//LCOV_EXCL_BR_LINE
    }

    //LCOV_EXCL_BR_START
    VSynch_load.write(VSynch.read());
    HSynch_load.write(HSynch.read());
    Height_load.write(Height);
    Width_load.write(Width);

    wait();
    //LCOV_EXCL_BR_STOP
  }
}

/*
** Generate static parameters for the disparty estimator's main fumnction
*/
void pre_process::parameters()
{
  // Variables declaration
  int hsize_tmp;
  int hd_tmp;
  int hbnd_tmp;
  int wsize_tmp;
  int wd_tmp;
  int wbnd_tmp;
  int height;
  int width;

  //Reset
  //LCOV_EXCL_BR_START
  VSynch_read=false;
  hsize.write(0);
  wsize.write(0);
  hd.write(0);
  wd.write(0);
  hbnd.write(0);
  wbnd.write(0);

  wait();
  //LCOV_EXCL_BR_STOP

  while(true){

    height = Height.read();//LCOV_EXCL_BR_LINE
    width  = Width.read();//LCOV_EXCL_BR_LINE

    VSynch_read   = VSynch;

    if(height==0){
      hsize_tmp = 0;
      hd_tmp    = 0;
      hbnd_tmp  = 0;
    }
    else{
      if(height%HEIGHT == 0)
        hsize_tmp =  height/HEIGHT;
      else
        hsize_tmp = (height/HEIGHT)+1;

      if(height%HEIGHT==0)
        hd_tmp = HEIGHT;
      else
        hd_tmp = HEIGHT-1;

      hbnd_tmp  = ((height - hd_tmp*hsize_tmp)/2) + (((height - hd_tmp*hsize_tmp)/2)%2);
    }

    if(width==0){
      wsize_tmp = 0;
      wd_tmp    = 0;
      wbnd_tmp  = 0;
    } else{
      if(width%WIDTH==0)
        wsize_tmp = width/WIDTH;
      else
        wsize_tmp = (width/WIDTH)+1;

      wd_tmp = WIDTH;

      wbnd_tmp  = ((width  - wd_tmp*wsize_tmp)/2) + (((width  - wd_tmp*wsize_tmp)/2)%2);
    }

    if((VSynch.read()==false)&&(VSynch_read==true)){
      //LCOV_EXCL_BR_START
      hsize.write(hsize_tmp);
      wsize.write(wsize_tmp);
      hd.write(hd_tmp);
      wd.write(wd_tmp);
      hbnd.write(hbnd_tmp);
      wbnd.write(wbnd_tmp);
      //LCOV_EXCL_BR_STOP
    }

    wait();//LCOV_EXCL_BR_LINE
  }   //end while

}//LCOV_EXCL_BR_LINE
