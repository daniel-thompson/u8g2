CFLAGS = -g -Wall -I../common/. -I../../../csrc/.

SRC = $(shell ls ../../../csrc/*.c) $(shell ls ../common/*.c ) main.c

OBJ = $(SRC:.c=.o)

u8g2_linux: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o u8g2_linux

clean:
	-rm $(OBJ) u8g2_linux

