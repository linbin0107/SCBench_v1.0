###################################################
## Description: Crossroad Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 10/19/2016
###################################################

TARGET = crossroad

default: $(TARGET)

include ../Makefile.common

$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@

