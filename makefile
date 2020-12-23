C_SOURCES = $(wildcard *.c */*.c)
HEADERS = $(wildcard *.h */*.h)
OBJ = $(C_SOURCES:.c = .o)
all: clean cync


%.o: %.c ${HEADERS}
	gcc -c $< -o $@







clean:
	rm -fr *.o *.c~
