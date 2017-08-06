##
# Set compilation flags, debug options etc.
include ./Makefile.conf
##

SRC_FILES := $(shell find src -name "*.c")
FREERTOS_FILES := $(FREERTOS_SRC)/tasks.c \
 $(FREERTOS_SRC)/queue.c \
 $(FREERTOS_SRC)/list.c \
 $(FREERTOS_SRC)/portable/GCC/ARM_CM3/port.c \
 $(FREERTOS_SRC)/portable/MemMang/heap_1.c
OBJ_FILES := $(patsubst %.c,%.o,$(patsubst src/%,obj/%,$(SRC_FILES)))
OBJ_FILES += $(patsubst %.c,%.o,$(patsubst third-party/%,obj/%,$(FREERTOS_FILES)))

INC := -I./inc \
 -I./third-party/freertos/FreeRTOS/Source/include \
 -I./third-party/freertos/FreeRTOS/Source/portable/GCC/ARM_CM3

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
	
clean :
	rm -r $(OBJ) $(BIN)
