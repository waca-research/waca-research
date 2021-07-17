## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS        += 
SMALL_CFLAGS  = -DSYNTHETIC1
MEDIUM_CFLAGS = -DALPACA
LARGE_CFLAGS  = -DBOOKCASE

########################################################################
## Targets
########################################################################
compile: large install

run-all: run-large

small: 
	$(CC) $(SMALL_CFLAGS) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o me-small.o $(LDFLAGS)

medium: 
	$(CC) $(MEDIUM_CFLAGS) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o me-medium.o $(LDFLAGS)

large: 
	$(CC) $(LARGE_CFLAGS) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o me-large.o $(LDFLAGS)

install:
	cp me-large.o ../../binary-default/

run-small:
	./me-small.o 2>&1 | tee ./output.small

run-medium:
	./me-medium.o 2>&1 | tee ./output.medium

run-large:
	./me-large.o 2>&1 | tee ./output.large

clean:
	rm -f *.o me-small me-medium me-large
