###########################################
##  Description: Makefile for fft
##  Author: Bin Lin
##  Affiliation: SVL Lab, Portland State University
##  Date: 09/01/2016
###########################################

TARGET = fft

default: $(TARGET)

include ../Makefile.common


$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@
