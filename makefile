SRC_DIR ?= ./src
BUILD_DIR ?= ./build
TARGET_EXEC ?= cync
C_SOURCES := $(shell find $(SRC_DIR) -name *.c)
C_BUILD_PATH = ${subst $(SRC_DIR),$(BUILD_DIR),$(C_SOURCES)}
OBJS = ${C_BUILD_PATH:.c=.o}

unix: CCFLAGS := -DUNIX -g -pthread
unix: all

win: CCFLAGS := -DWIN32 -g -lws2_32
win: all

debug: CCFLAGS := -DDEBUG -g
debug: all

test: CCFLAGS := -DTEST -g
test: all

all: clean $(BUILD_DIR)/$(TARGET_EXEC)
	cp configuration.txt ./build/configuration.txt

$(BUILD_DIR)/$(TARGET_EXEC): ${OBJS}
	$(CC) -o $@ $^ $(CCFLAGS)

$(BUILD_DIR)/%.o: ${SRC_DIR}/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@
clean:
	rm -rf build

MKDIR_P := mkdir -p
