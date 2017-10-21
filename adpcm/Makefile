####################################################
##  Description: Makefile for adpcm_encoder
##  Author: Bin Lin
##  Affiliation: SVL Lab, Portland State University
##  Date: 09/01/2016
####################################################

TARGET = adpcm

default: $(TARGET)

include ../Makefile.common


$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@
