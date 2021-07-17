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
compile: large install

run-all: run-large

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o basicmath_large.o $(LDFLAGS)

install:
	cp basicmath_large.o ../../binary-default/

run-large:
	./basicmath_large.o 2>&1 | tee ./output.txt

clean:
	-rm -f *.o basicmath_large output*
