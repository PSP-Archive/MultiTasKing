TARGET = multiTasKing
OBJS = main.o libs/minIni/minIni.o libs/id3/id3.o libs/id3/opendir.o libs/id3/mem64.o libs/miniConv/miniconv.o libs/supportlib/supportlib.o

MYLIBS=
STDLIBS= -lpspgum -lpspgu -lm -losl -lpng -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg -lUnrar -lstdc++ -lzzip -lpspaudiocodec -lpspusb -lpspusbstor
LIBS=$(STDLIBS) $(MYLIBS)

LIBDIR =
BUILD_PRX = 1
PSP_FW_VERSION=371

PSP_LARGE_MEMORY = 1#######################

PRX_EXPORTS = exports.exp
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = by Carlosgs
PSP_EBOOT_ICON = ICON0.png
PSPSDK=$(shell psp-config --pspsdk-path)
PSPLIBSDIR = $(PSPSDK)/..
include $(PSPSDK)/lib/build.mak

stub: exports.exp
	$(shell psp-config --pspdev-path)/bin/psp-build-exports --build-stubs $<
	#mv multiTasKingLib.S ./Programas/multiTasKingLib.S

#PSP_LARGE_MEMORY = 1#######################
