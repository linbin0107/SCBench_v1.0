###################################################
## noc Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 07/31/2016
###################################################

TARGET = noc

default: $(TARGET)

include ../Makefile.common

$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@

