###########################################
##  Description: Makefile for tlm_nb_transport
##  Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
##  Date: 10/25/2016
###########################################

TARGET = tml_nb_transport

default: $(TARGET)

include ../Makefile.common

INC_DIRS += -I$(SYSTEMC_HOME)/include/tlm_core

$(OBJS): %.o: %.cpp
	$(CXX) $(INC_DIRS) $(CFLAGS) -c $< -o $@
