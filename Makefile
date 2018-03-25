##
# Set compilation flags, debug options etc.
include ./Makefile.conf
##
.PHONY: test

##
# Convert resource files to C
##
RESOURCE_FILES := $(shell find src -name "*.html" -o -name "*.js" -o -name "*.css")
RESOURCE_C_FILES := $(patsubst %.css, %.c,\
$(patsubst %.js,%.c,\
$(patsubst %.html,%.c,\
$(patsubst src/resources/%,src/resources/generated/%,$(RESOURCE_FILES)))))

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

resources : $(RESOURCE_C_FILES)

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
	
src/resources/generated/html/%.c : src/resources/html/%.html
	@echo "BIN2C $< -> $@"
	@mkdir -p $(@D)
	@mkdir -p $(patsubst src/%,inc/%,$(@D))
	@bin2c -d $(patsubst src/%,inc/%,$@) -o $@ $<
	
src/resources/generated/js/%.c : src/resources/js/%.js
	@echo "BIN2C $< -> $@"
	@mkdir -p $(@D)
	@mkdir -p $(patsubst src/%,inc/%,$(@D))
	@bin2c -d $(patsubst src/%,inc/%,$@) -o $@ $<
	
src/resources/generated/css/%.c : src/resources/css/%.css
	@echo "BIN2C $< -> $@"
	@mkdir -p $(@D)
	@mkdir -p $(patsubst src/%,inc/%,$(@D))
	@bin2c -d $(patsubst src/%,inc/%,$@) -o $@ $<
	
test :
	@mkdir -p bin/test
	gcc -I./inc src/time/epoch.c src/time/utc.c test/time/conversion.c -o bin/test/conversion-test
	
clean :
	rm -r $(OBJ) $(BIN) src/resources/generated inc/resources/generated
