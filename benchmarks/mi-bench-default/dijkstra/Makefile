## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SMALL_SRC_FILES  += $(SRC_FILES) dijkstra_small.c
LARGE_SRC_FILES  += $(SRC_FILES) dijkstra_large.c

## CFlags 
CFLAGS     += 

########################################################################
## Targets
########################################################################
compile: large install

run-all: run-large

# small: 
# 	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(SMALL_SRC_FILES) -o dijkstra_small $(LDFLAGS)

large: 
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $(LARGE_SRC_FILES) -o dijkstra_large.o $(LDFLAGS)

install:
	cp dijkstra_large.o ../../binary-default/

run-small:
	./dijkstra_small.o input.dat 2>&1 | tee ./output.txt

run-large:
	./dijkstra_large.o input.dat 2>&1 | tee ./output.txt

clean:
	rm -f *.o dijkstra_small dijkstra_large output*
