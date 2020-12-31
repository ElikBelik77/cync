SRC_DIR ?= ./src
BUILD_DIR ?= ./build
TARGET_EXEC ?= cync
C_SOURCES := $(shell find $(SRC_DIR) -name *.c)
C_BUILD_PATH = ${subst $(SRC_DIR),$(BUILD_DIR),$(C_SOURCES)}
OBJS = ${C_BUILD_PATH:.c=.o}
all: release
debug: clean $(BUILD_DIR)/$(TARGET_EXEC)
	cp configuration.txt ./build/configuration.txt
	
release: clean $(BUILD_DIR)/$(TARGET_EXEC)
	mkdir -p /usr/local/etc/cync
	cp configuration.txt /usr/local/etc/cync/configuration.txt
	cp $(BUILD_DIR)/$(TARGET_EXEC) /usr/local/bin/$(TARGET_EXEC)
	cp cyncservice /etc/init.d/cyncservice

$(BUILD_DIR)/$(TARGET_EXEC): ${OBJS}
	gcc -pthread -o $@ $^

$(BUILD_DIR)/%.o: ${SRC_DIR}/%.c
	$(MKDIR_P) $(dir $@)
	gcc -c $< -o $@
clean:
	rm -rf build	
	rm /etc/init.d/cyncservice
	rm /usr/local/etc/cync/configuration.txt
	rm /usr/local/bin/$(TARGET_EXEC)
MKDIR_P := mkdir -p
