###########################################
##  Description: Makefile for UART
##  Author: Bin Lin
##  Affiliation: SVL Lab, Portland State University
##  Date: 09/01/2016
###########################################

TARGET = uart

default: $(TARGET)

include ../Makefile.common


$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@
