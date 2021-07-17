## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS     += -DUSERS=10 -DTEST_USERS=10 -DMOVIES=10 -DLOOPS=20

########################################################################
## Targets
########################################################################
compile: large install

run-all: run-small run-medium run-large

small: 
	$(CC) $(CFLAGS) $(APP_FLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o rbm-small.o $(LDFLAGS)

medium: 
	$(CC) $(CFLAGS) $(APP_FLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o rbm-medium.o $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(APP_FLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o rbm-large.o $(LDFLAGS)

install:
	cp rbm-large.o ../../binary-default/

run-small:
	-./rbm-small.o | tee ./outputs.small

run-medium:
	-./rbm-medium.o  | tee ./outputs.medium

run-large:
	-./rbm-large.o  | tee ./outputs.large

clean:
	-rm -f rbm-small rbm-medium rbm-large out.txt *.o outputs*
