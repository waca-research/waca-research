## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SMALL_SRC_FILES  += $(SRC_FILES) qsort_small.c
LARGE_SRC_FILES  += $(SRC_FILES) qsort_large.c

## CFlags 
CFLAGS     += -static

########################################################################
## Targets
########################################################################
compile: large install

run-all: run-large

# small: 
# 	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SMALL_SRC_FILES) -o qsort_small $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(LARGE_SRC_FILES) -o qsort_large.o $(LDFLAGS)

install:
	cp qsort_large.o ../../binary-default/

run-small:
	./qsort_small.o input_small.dat 2>&1 | tee ./output.txt

run-large:
	./qsort_large.o input_large.dat 2>&1 | tee ./output.txt

clean:
	rm -f *.o qsort_small qsort_large output*
