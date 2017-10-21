###########################################
##  Description: Makefile for snow3G
##  Author: Bin Lin
##  Affiliation: SVL Lab, Portland State University
##  Date: 09/01/2016
###########################################

TARGET = snow3G

default: $(TARGET)

include ../Makefile.common


$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@
