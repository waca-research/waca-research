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
compile: rijndael install

run-all: run-small run-large

rijndael: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o rijndael.o $(LDFLAGS)

install:
	cp rijndael.o ../../binary-default/

run-small:
	./rijndael.o input_small.asc output_small.enc e  1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 2>&1 | tee ./output.txt
	./rijndael.o output_small.enc output_small.dec d 1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 2>&1 | tee ./output.txt


run-large:
	./rijndael.o input_large.asc output_large.enc e  1234567890abcdeffedcba09876543211234567890abcdeffedcba0987654321 2>&1 | tee ./output.txt

clean:
	rm -f *.o rijndael.o output*
