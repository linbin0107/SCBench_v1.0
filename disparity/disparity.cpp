//---------------------------------------------------------------------------
//
//
// File Name    : disparity_kernel.cpp
// Description  : 3D disparity estimation
// Release Date : 16/07/13
// Author       :
//
// Revision History
//---------------------------------------------------------------------------
// Date     Author      Version     Description
//---------------------------------------------------------------------------
//16/07/13
//---------------------------------------------------------------------------
#include "disparity.h"


//---------------------------
// Main disparity estimatation function
//    1.- Get the difference between Right and Left pixel and adjust
//    2.- Calculate the sum of absolute differences (SAD)
//    3.- Find min sad for each image window and generate output
//--------------------------
void disparity::run()
{
  // Variables declaration
  int sum_all;
  int x, y;
  bool VSynch_tmp, HSynch_pulse_tmp;

  //LCOV_EXCL_BR_START
  sc_uint<7> hd_tmp;
  // Reset - should be executable in 1 clock cycle
  valid_out.write(0);
  depth_out.write(0);
  //LCOV_EXCL_BR_STOP

  // counters initializations
  count_hd = 0;
  count_wd = 0;
  count_delay = 0;
  count_hsize = 0;
  count_wsize = 0;
  count_pixel = 0;
  count_line = 0;
  count_skip_line = 0;
  count_skip_pixel = 0;

  // Arrays can't be mapped to memories if initialization is needed
  for(x = 0;x < MAX_DIFF; x++) {
    tmp_sum[x] = 0;
    final_sum[x] = 0;
    Left_buffer [x]=0;
  }

  for(y = 0; y< FULL; y++) {
     Right_buffer[y]=0;
  }

  wait();//LCOV_EXCL_BR_LINE
  // --- Main disparity estiation computational loop
  while(1) {

    // Wait for synchronization signal
    VSynch_tmp = VSynch.read();//LCOV_EXCL_BR_LINE
    while(VSynch_tmp == 1) {
      wait();//LCOV_EXCL_BR_LINE
      VSynch_tmp = VSynch.read();//LCOV_EXCL_BR_LINE
    }

    // Skip boundary lines
    count_skip_line = 0;

    while(count_skip_line  <= hbnd.read() ) {
      VSynch_tmp = VSynch.read();//LCOV_EXCL_BR_LINE
      HSynch_pulse_tmp = HSynch_pulse.read();//LCOV_EXCL_BR_LINE
      if(VSynch_tmp==0 && HSynch_pulse_tmp==1) {
        count_skip_line++;
      }

      wait();//LCOV_EXCL_BR_LINE
    }

    // Start a new frame
    hd_tmp = hd.read();//LCOV_EXCL_BR_LINE
    while(count_hd < hd_tmp) {
      // Start a new block
      for(count_hsize = 0; count_hsize <  hsize.read();
          count_hsize++) {
        // Clean up sad memory
        count_delay=0;
        if(count_hsize == 0){
	      while(count_delay < FULL){
           for(x = 0; x< MAX_DIFF; x++) {
	         sad[x][count_delay]=0;
	       }

	       count_delay++;
         }
	   }

        // Start a new line
        // Skip boundary pixels
        count_skip_pixel = 0;
        while(count_skip_pixel <  wbnd.read()  ) {
          if(valid_in.read()==1) {
            count_skip_pixel++;
          }
          wait();//LCOV_EXCL_BR_LINE
        }

        count_skip_pixel = 0;
        count_delay = 0;

	    // -- Read new Left and Right pixels into buffers
        bool valid_in_tmp = valid_in.read();//LCOV_EXCL_BR_LINE
        sc_uint<7> wd_tmp = wd.read();//LCOV_EXCL_BR_LINE
        while (valid_in_tmp==1 || count_wd  < wd_tmp){
          for(x = 1 ;x < FULL; x++) {
	        Right_buffer[x-1] = Right_buffer[x];
	      }
	      Right_buffer[FULL-1] = Right_pixel_Y.read();//LCOV_EXCL_BR_LINE

          if(count_delay == 0){
	        for(x = 1 ; x < MAX_DIFF; x++) {
	          Left_buffer[x] = Left_pixel_Y.read();//LCOV_EXCL_BR_LINE
	        }
	      } else {
  	        for(x = 1 ;x < MAX_DIFF;x++) {
	          Left_buffer[x-1] = Left_buffer[x];
	        }
	        Left_buffer[MAX_DIFF-1] = Left_pixel_Y.read();//LCOV_EXCL_BR_LINE
	      }

          if(valid_in.read() == 1 && count_delay < FULL-1) {
	        count_delay++;
	      }
          else if(count_delay >= FULL-1 || count_pixel >= MAX_DIFF*8) {
            // Calculate pixel differences
	        bool t = (count_wsize == wsize.read()-1);
            for(x = 0; x < MAX_DIFF; x++) {
              pixel_Ly = Left_buffer[x];
              pixel_Ry = Right_buffer[0];
              // Compute the absolute value of the Left and Right pixels
	         if(pixel_Ly > pixel_Ry)
		        pixel_LR_diff = pixel_Ly-pixel_Ry;
	         else
                pixel_LR_diff = pixel_Ry-pixel_Ly;

	         // Adjust the pixel difference
             if(pixel_LR_diff< DIST_ADJUST_LOW )
                pixel_LR_diff_adj = pixel_LR_diff;
             else if(DIST_ADJUST_HIGH <= pixel_LR_diff )
                pixel_LR_diff_adj = pixel_LR_diff * 2;
             else
                pixel_LR_diff_adj = pixel_LR_diff;

	         // Sum up the differences
             tmp_sum[x] = tmp_sum[x]+ MIN(127,pixel_LR_diff_adj);
             sum_all = tmp_sum[x]+final_sum[x];

             if(t) {
		        sad[x][count_wd]=sum_all ;
                tmp_sum[x] = 0;
	         } else if(count_wsize == 0){
                final_sum[x] = sad[x][count_wd];
	         }
	       }  // end for loop MAX_DIFF

            // Update control counters
            count_wsize++;
            count_pixel++;

            if(count_wsize == wsize.read()) {
              count_wd ++;
              count_wsize = 0;
            }
	      }

          //LCOV_EXCL_BR_START
	      wait();

          valid_in_tmp = valid_in.read();
          wd_tmp = wd.read();
          //LCOV_EXCL_BR_STOP
        } // while input valid = 1

        count_line ++;
        count_wd = 0;
        count_pixel = 0;
        count_delay = 0;

        wait();//LCOV_EXCL_BR_LINE
      } //for loop hsize_cnt < hsize.read()

      count_hd++;
      level = FULL;

      // Find min sad for each block and generate output
      for(x = 0; x<64 ; x++) {
        for(y = 0; y < MAX_DIFF; y++) {
          sad_single[y]=sad[y][x];
        }

	    Lsad = sad_single[FULL];
        level = FULL;

        for(y = 1; y< FULL; y++) {
          if(sad_single[FULL-y] < Lsad) {
            Lsad = sad_single[FULL-y];
            level = 64-y;
          }
          if(sad_single[FULL+y] < Lsad) {
            Lsad = sad_single[FULL+y];
            level = FULL+y;
          }
        }

        if(sad_single[0] < Lsad) {
          level = 0;
        }
        minD = level - FULL;

        out = (int)(MAX(0,MIN(255,minD* ADJ + 128)))& 0xFF;

        //LCOV_EXCL_BR_START
        valid_out.write(1);
        depth_out.write(out);

        wait();
        valid_out.write(0);
        //LCOV_EXCL_BR_STOP
      }

      hd_tmp = hd.read();//LCOV_EXCL_BR_LINE
    }//end while count_hd

    count_hd = 0;

    VSynch_tmp = VSynch.read();//LCOV_EXCL_BR_LINE
    while(VSynch_tmp == 0) {
      wait();//LCOV_EXCL_BR_LINE
      VSynch_tmp = VSynch.read();//LCOV_EXCL_BR_LINE
    }
  }// end of while(1)
}//end of run


int disparity::MAX(int a, int b){
  return a>b?a:b;
}

int disparity::MIN(int a, int b){
  return a>b?b:a;

}

//---------------------------
// Pulse generator to control the estimation process
//--------------------------
void disparity::HSynch_control() {
    //LCOV_EXCL_BR_START
    HSynch_pulse.write(0);
    HSynch_d1.write(0);
    wait();
    //LCOV_EXCL_BR_STOP
    while(1){
      HSynch_d1.write(HSynch.read());//LCOV_EXCL_BR_LINE

      if(HSynch_d1.read()==1 && HSynch.read()==0) {
        HSynch_pulse.write(1);//LCOV_EXCL_BR_LINE
      } else {
        HSynch_pulse.write(0);//LCOV_EXCL_BR_LINE
      }

      wait();//LCOV_EXCL_BR_LINE
   }
}//LCOV_EXCL_BR_LINE
