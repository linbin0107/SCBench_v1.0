###################################################
## absasr Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 07/31/2016
###################################################

TARGET = simple_abs

include ../Makefile.common

$(OBJS): %.o : %.cpp
	$(CXX) $(CFLAGS) $(INC_DIRS) -c $< -o $@
