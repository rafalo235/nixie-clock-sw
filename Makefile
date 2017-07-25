##
# Set compilation flags, debug options etc.
include ./Makefile.conf
##

all : freertos build
	$(CC) $(CFLAGS) $(LDFLAGS) src/main-loop.c startup.s -o build/nixie-clock-sw.elf

hex : all
	$(OBJCOPY) -O ihex build/nixie-clock-sw.elf build/nixie-clock-sw.hex

freertos :
	@echo "Building FreeRTOS"

build :
	@echo "Making directory 'build'"
	@mkdir build
