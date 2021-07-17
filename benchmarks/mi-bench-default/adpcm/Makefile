## Specify the directory which contains common files
COMMON_DIR = ../../common

## Include the common Makefile
include $(COMMON_DIR)/common.mk

## Specify source files
SRC_FILES  +=

## CFlags
CFLAGS     += -static

########################################################################
## Targets
########################################################################

ALLSRC= $(INCLUDE_DIRS) README adpcm.2 Makefile adpcm.h libst.h adpcm.c timing.c rawcaudio.c \
	rawdaudio.c suncaudio.c sgicaudio.c sgidaudio.c hello.adpcm.uu 

all: adpcm.o rawcaudio rawdaudio timing install
	cp rawcaudio_exe.o rawdaudio timing ../../binary-default/

sgi: all sgicaudio sgidaudio

sun: all suncaudio sundaudio

adpcm.o: adpcm.h $(SRC_FILES)

suncaudio: suncaudio.o adpcm.o
	$(CC) $(INCLUDE_DIRS) adpcm.o suncaudio.o -o suncaudio -L/usr/demo/SOUND -laudio

sundaudio: suncaudio
	cp suncaudio sundaudio

install:
	cp rawcaudio_exe.o ../../binary-default/

sgicaudio: sgicaudio.o adpcm.o
	$(CC) $(INCLUDE_DIRS) sgicaudio.o adpcm.o -o sgicaudio -laudiofile -lm

sgidaudio: sgidaudio.o adpcm.o
	$(CC) $(INCLUDE_DIRS) sgidaudio.o adpcm.o -o sgidaudio -laudiofile -lm

rawcaudio: rawcaudio.o adpcm.o
	$(CC) $(INCLUDE_DIRS) $(SRC_FILES) $(CFLAGS) rawcaudio.o adpcm.o -o rawcaudio_exe.o $(LDFLAGS)

rawdaudio: rawdaudio.o adpcm.o
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) rawdaudio.o adpcm.o -o rawdaudio

timing: timing.o adpcm.o
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) timing.o adpcm.o -o timing

adpcm.shar: $(ALLSRC)
	shar $(ALLSRC) > adpcm.shar

clean:
	rm -f *.o ; rm -f rawcaudio_exe rawdaudio timing ; rm -f ./bin/* ./output*
