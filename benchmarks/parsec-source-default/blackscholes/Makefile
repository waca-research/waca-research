PREFIX=${PARSECDIR}/pkgs/apps/blackscholes/inst/${PARSECPLAT}

CSRC    = $(SRC)
TARGET  = blackscholes
M4_BASE = .
MACROS  = c.m4.pthreads
version = pthreads

ifdef source
	ifeq "$(source)" "simd" 
		SRC = blackscholes.simd.c
		CXXFLAGS += -msse3
	endif
else
	SRC	= blackscholes.c
endif

ifdef version
  ifeq "$(version)" "pthreads"
    M4_SRC    = blackscholes.m4.cpp
    CSRC      = $(M4_SRC)
    MT        = -DENABLE_THREADS
    CXXFLAGS += -pthread
  endif
  ifeq "$(version)" "openmp"
    MT        = -DENABLE_OPENMP
  endif
  ifeq "$(version)" "tbb"
    MT        = -DENABLE_TBB
  endif
else
  MT        = 
endif



# Default build single precision version
NCO     = -DNCO=4

ifdef chk_err
ERR     = -DERR_CHK
endif

ifdef single
NCO = -DNCO=4
endif

ifdef size
SZ = -DN=$(size)
else
SZ = -DN=960
endif

ifdef double
NCO = -DNCO=2
endif

## CXXFLAGS += $(MT) $(NCO) $(FUNC) $(ERR) $(CSRC) log_data.c
## 
## all: $(TARGET)
## 
## $(TARGET): clean $(CSRC)
## 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -static -O3 -o $(TARGET) -I/usr/local/include -L/usr/local/lib/libpapi.a -lrt -lpapi -pthread
## 
## $(M4_SRC): $(SRC)
## 	m4 $(M4_BASE)/$(MACROS) $< > $@

## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  += blackscholes.m4.cpp

## CFlags 
CXXFLAGS   += $(MT) $(NCO) $(FUNC) $(ERR)

########################################################################
## Targets
########################################################################
compile: blackscholes install

run-all: run-2T run-4T run-8T

blackscholes: 
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o blackscholes.o $(LDFLAGS)

# m4:
# 	m4 ./c.m4.pthreads blackscholes.cpp > blackscholes.m4.cpp

install:
	cp blackscholes.o ../../binary-default/

run-2T:
	./blackscholes.o 2 ./in_16K.txt ./output.txt

run-4T:
	./blackscholes.o 4 ./in_16K.txt ./output.txt

run-8T:
	./blackscholes.o 8 ./in_16K.txt ./output.txt

clean:
	rm -f *.o out*
