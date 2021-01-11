SRC_DIR ?= ./src
INC_DIR ?= ./inc
BUILD_DIR ?= ./build
TARGET_EXEC ?= cync
C_SOURCES := $(shell find $(SRC_DIR) -name *.c)
HEADERS := $(shell find $(INC_DIR) -name *.h)
C_BUILD_PATH = ${subst $(SRC_DIR),$(BUILD_DIR),$(C_SOURCES)}
OBJS = ${C_BUILD_PATH:.c=.o}
CCFLAGS ?= -g -pedantic -I $(INC_DIR)
ifeq ($(OS), UNIX)
	CCFLAGS += -DUNIX -pthread
endif
ifeq ($(OS), WIN32)
	CCFLAGS += -DWIN32 -lws2_32
endif

debug: CCFLAGS += -DDEBUG
debug: all

test: CCFLAGS += -DTEST
test: all

all: clean $(BUILD_DIR)/$(TARGET_EXEC)
	cp configuration.txt ./build/configuration.txt

$(BUILD_DIR)/$(TARGET_EXEC): ${OBJS}
	$(CC) -o $@ $^ $(CCFLAGS)

$(BUILD_DIR)/%.o: ${SRC_DIR}/%.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CCFLAGS) -c $^ -o $@
clean:
	rm -rf build
	find . -name *.save | xargs rm -f

format:
		clang-format -i -style=file src/*.c inc/*.h

MKDIR_P := mkdir -p
