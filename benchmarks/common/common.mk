############################################################
# Set the platform option
# Valid values are "odroid", "xavier_nx", "agx_xavier"
############################################################
PLATFORM     = xavier_nx

############################################################
# Set the governor
############################################################
GOVERNOR     = ondemand

############################################################
# Set the algorithm
############################################################
ALGORITHM    = binary-default

############################################################
# Set other variables
############################################################
CUR_DIR      = $(shell pwd)
SRC_FILES    := $(COMMON_DIR)/src/log_data_$(PLATFORM).c $(COMMON_DIR)/src/papi_functions_${PLATFORM}.c

############################################################
# Set path to the PAPI library
############################################################
PAPI_PATH    = /usr/local/lib/libpapi.a

############################################################
# Set warning flags to be ignored
############################################################
IGNORE_WARNING_FLAGS = -Wno-unused-result -Wno-implicit-function-declaration -Wno-int-conversion

############################################################
# Set compiler, compilation options, linking options
############################################################
CC           = gcc
CFLAGS       = -O3 -static $(IGNORE_WARNING_FLAGS) -D$(PLATFORM) -D_POSIX_SOURCE -D_GNU_SOURCE
LDFLAGS      = $(PAPI_PATH) -lrt -pthread -lm 

INCLUDE_DIRS = -I$(CUR_DIR) -I$(COMMON_DIR)/src

############################################################
# Specify targets
############################################################

