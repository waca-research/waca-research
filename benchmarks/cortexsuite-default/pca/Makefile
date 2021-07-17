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
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o pca-small.o $(LDFLAGS)

medium: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o pca-medium.o $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o pca-large.o $(LDFLAGS)

install:
	cp pca-small.o pca-medium.o pca-large.o ../../binary-default/

run-small:
	-./pca-small.o small.data 1593 256 R 2>&1 | tee ./output.small

run-medium:
	-./pca-medium.o medium.data 722 800 R 2>&1 | tee ./output.medium

run-large:
	-./pca-large.o large.data 5000 1059 R 2>&1 | tee ./output.large

clean:
	-rm -f *.o pca-small pca-medium pca-large output*
