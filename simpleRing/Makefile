###################################################
## Description: Simple ring Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 10/19/2016
###################################################

TARGET = simpleRing

default: $(TARGET)

include ../Makefile.common

$(OBJS): %.o: %.cpp
	$(CXX) $(INC_DIRS) $(CFLAGS) -c $< -o $@

