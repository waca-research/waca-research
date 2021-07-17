## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS     += 

########################################################################
## Targets
########################################################################
compile: small medium large install

run-all: run-small run-medium run-large

small: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o spc-small.o $(LDFLAGS)

medium: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o spc-medium.o $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o spc-large.o $(LDFLAGS)

install:
	cp spc-small.o spc-medium.o spc-large.o ../../binary-default/

run-small:
	-./spc-small.o datasets/R15 600 2 15 0.707 1 2>&1 | tee ./output.small

run-medium:
	-./spc-medium.o datasets/Aggregation 788 2 7 0.707 1 2>&1 | tee ./output.medium

run-large:
	-./spc-large.o datasets/D31 1600 2 16 0.707 1 2>&1 | tee ./output.large


clean:
	-rm -f *.o spc-small spc-medium spc-large output*

