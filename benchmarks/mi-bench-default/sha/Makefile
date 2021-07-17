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
compile: sha install

run-all: run-small run-large

sha: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o sha.o $(LDFLAGS)

install:
	cp sha.o ../../binary-default/

run-small:
	./sha.o input_small.asc 2>&1 | tee ./output.txt

run-large:
	./sha.o input_large.asc 2>&1 | tee ./output.txt

clean:
	rm -f *.o sha output*
