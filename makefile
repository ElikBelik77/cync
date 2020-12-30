SRC_DIR ?= ./src
BUILD_DIR ?= ./build
TARGET_EXEC ?= cync
C_SOURCES := $(shell find $(SRC_DIR) -name *.c)
C_BUILD_PATH = ${subst $(SRC_DIR),$(BUILD_DIR),$(C_SOURCES)}
OBJS = ${C_BUILD_PATH:.c=.o}

all: clean $(BUILD_DIR)/$(TARGET_EXEC)
	cp configuration.txt ./build/configuration.txt
$(BUILD_DIR)/$(TARGET_EXEC): ${OBJS}
	gcc -pthread -o $@ $^

$(BUILD_DIR)/%.o: ${SRC_DIR}/%.c
	$(MKDIR_P) $(dir $@)
	gcc -c $< -o $@
clean:
	rm -rf build	
MKDIR_P := mkdir -p
