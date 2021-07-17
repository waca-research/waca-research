## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += 

## CFlags 
CFLAGS     += 
CXXFLAGS   += -Wno-invalid-offsetof

## Additional parameters
TARGET      = fluidanimate
version     = pthreads
OBJS        = pthreads.o cellpool.o parsec_barrier.o
CXXFLAGS    += -pthread -D_GNU_SOURCE -D__XOPEN_SOURCE=600	

########################################################################
## Targets
########################################################################
compile: clean fluidanimate install

run-all: run-2T run-4T run-8T

fluidanimate: pthreads fluidcmp

pthreads: $(OBJS)
	$(CXX) $(INCLUDE_DIRS) $(CFLAGS) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(SRC_FILES) -o $(TARGET).o 

%.o : %.cpp
	$(CXX) $(INCLUDE_DIRS) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(SRC_FILES) -c $<

fluidcmp: fluidcmp.cpp
	rm -rf fluidcmp.o
	$(CXX) $(INCLUDE_DIRS) $(CFLAGS) $(CXXFLAGS) fluidcmp.cpp -o fluidcmp.o

install:
	cp fluidanimate.o ../../binary-default/

run-2T:
	./fluidanimate.o 2 1 ./in_35K.fluid ./output.txt

run-4T:
	./fluidanimate.o 4 1 ./in_35K.fluid ./output.txt

run-8T:
	./fluidanimate.o 8 1 ./in_35K.fluid ./output.txt

clean:
	rm -rf $(TARGET).o
	rm -rf *.o
	rm -rf out*

