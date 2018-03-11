##
# Set compilation flags, debug options etc.
include ./Makefile.conf
##
.PHONY: test

SRC_FILES := $(shell find src -name "*.c")
FREERTOS_FILES := $(FREERTOS_SRC)/tasks.c \
 $(FREERTOS_SRC)/queue.c \
 $(FREERTOS_SRC)/list.c \
 $(FREERTOS_SRC)/portable/GCC/ARM_CM3/port.c \
 $(FREERTOS_SRC)/portable/MemMang/heap_1.c
ESP8266_AT_LIB_FILES := $(ESP8266_AT_LIB_SRC)/buffer.c \
 $(ESP8266_AT_LIB_SRC)/esp8266.c
UCHTTPSERVER_FILES := $(UCHTTPSERVER_SRC)/uchttpserver.c
OBJ_FILES := $(patsubst %.c,%.o,$(patsubst src/%,obj/%,$(SRC_FILES)))
OBJ_FILES += $(patsubst %.c,%.o,$(patsubst third-party/%,obj/%,$(FREERTOS_FILES)))
OBJ_FILES += $(patsubst %.c,%.o,$(patsubst third-party/%,obj/%,$(ESP8266_AT_LIB_FILES)))
OBJ_FILES += $(patsubst %.c,%.o,$(patsubst third-party/%,obj/%,$(UCHTTPSERVER_FILES)))

INC := -I./inc \
 -I./inc/port/esp8266-at-lib \
 -I./inc/port/freertos \
 -I./third-party/freertos/FreeRTOS/Source/include \
 -I./third-party/freertos/FreeRTOS/Source/portable/GCC/ARM_CM3 \
 -I./third-party/esp8266-at-lib/00-ESP8266_LIBRARY \
 -I./third-party/uChttpserver/inc

all : $(OBJ_FILES)
	@mkdir -p $(BIN)
	@echo "LD $(BIN)/nixie-clock-sw.elf"
	@$(CC) $(LDFLAGS) $(OBJ_FILES) startup.s -o $(BIN)/$(APP).elf

hex : all
	$(OBJCOPY) -O ihex $(BIN)/$(APP).elf $(BIN)/$(APP).hex

debug : all
	$(DEBUGER) $(BIN)/$(APP).elf -x .gdbscript

$(OBJ)/%.o : src/%.c
	@echo "CC $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
$(OBJ)/%.o : third-party/%.c
	@echo "CC $< -> $@"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	
test :
	@mkdir -p bin/test
	gcc -I./inc src/time/epoch.c src/time/utc.c test/time/conversion.c -o bin/test/conversion-test
	
clean :
	rm -r $(OBJ) $(BIN)
