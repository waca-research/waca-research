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
compile: fft install

run-all: run

fft: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o fft.o $(LDFLAGS)

install:
	cp fft.o ../../binary-default/

run:
	./fft.o 8 32768 2>&1 | tee ./output.txt

clean:
	-rm -f *.o fft output*
