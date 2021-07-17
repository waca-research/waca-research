## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS     += -static

########################################################################
## Targets
########################################################################
compile: patricia install

run-all: run-small run-large

patricia: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o patricia.o $(LDFLAGS)

install:
	cp patricia.o ../../binary-default/

run-small:
	./patricia.o small.udp 2>&1 | tee ./output.txt

run-large:
	./patricia.o large.udp 2>&1 | tee ./output.txt

clean:
	rm -f *.o patricia output*
