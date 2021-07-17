## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS     += -std=c99 

########################################################################
## Targets
########################################################################
compile: small install 

run-all: run-small 

small: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o lenet5.o $(LDFLAGS)

install:
	cp lenet5.o ../../binary-default/

run-small:
	-./lenet5.o

clean:
	-rm -f *.o

