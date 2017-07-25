##
# Set compilation flags, debug options etc.
include ./Makefile.conf
##

all : freertos build
	$(CC) $(CFLAGS) $(LDFLAGS) src/main-loop.c startup.s -o build/nixie-clock-sw.elf

freertos :
	@echo "Building FreeRTOS"

build :
	@echo "Making directory 'build'"
	@mkdir build
