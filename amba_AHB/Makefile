###################################################
## amba_ahb Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 07/31/2016
###################################################

TARGET = amba_ahb

include ../Makefile.common

SYSC_FLAGS = -DSYSC -DSC_INCLUDE_DYNAMIC_PROCESSES
LIBS += -lm

$(OBJS): %.o: %.cpp
	$(CXX) $(SYSC_FLAGS) $(INC_DIRS) $(CFLAGS) -c $< -o $@
