###################################################
## md5c Makefile
## Author: Bin Lin
## Affiliation: SVL Lab, Portland State University
## Date: 08/04/2016
###################################################

TARGET = md5c

default: $(TARGET)

include ../Makefile.common

$(OBJS): %.o: %.cpp
	$(CC) $(INC_DIRS) $(CFLAGS) -c $< -o $@

