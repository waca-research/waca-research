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
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -g -o kmeans-small.o $(LDFLAGS)

medium: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o kmeans-medium.o $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o kmeans-large.o $(LDFLAGS)

install:
	cp kmeans-small.o kmeans-medium.o kmeans-large.o ../../binary-default/

run-small:
	-./kmeans-small.o ./datasets/D31 1024 512 256 2>&1 | tee ./output.small

run-medium:
	-./kmeans-medium.o ./datasets/finland 13467 2 15 2>&1 | tee ./output.medium

run-large:
	-./kmeans-large.o ./datasets/MINST 10000 748 10 2>&1 | tee ./output.large

clean:
	-rm -f *.o kmeans-small kmeans-medium kmeans-large output*

