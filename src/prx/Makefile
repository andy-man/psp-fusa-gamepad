TARGET = usbgamepad
OBJS   = usbhid.o
LIBS   = -lpspusb_driver -lpspusbbus_driver -lpspdisplay_driver

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

BUILD_PRX = 1

PRX_EXPORTS = exports.exp

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LDFLAGS = -nostartfiles
LIBDIR =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
