## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += $(wildcard $(CURDIR)/*.c)

## CFlags 
CFLAGS     += -fopenmp

########################################################################
## Targets
########################################################################
compile: large install

run-all: run-1T run-2T run-4T run-6T

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -g -o vgg16.o -lm $(LDFLAGS)

install:
	cp vgg16.o ../../binary-default/

run-1T:
	./vgg16.o 1 filelist.txt results-1T.txt

run-2T:
	./vgg16.o 2 filelist.txt results-2T.txt

run-4T:
	./vgg16.o 4 filelist.txt results-4T.txt

run-6T:
	./vgg16.o 6 filelist.txt results-6T.txt

clean:
	-rm -f *.o results* 
