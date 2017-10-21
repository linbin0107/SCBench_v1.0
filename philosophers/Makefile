###################################################
## Description: Philosophers Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 10/19/2016
###################################################

TARGET = philosophers

default: $(TARGET)

include ../Makefile.common

$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@

