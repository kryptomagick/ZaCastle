CC=gcc
CFLAGS=-O3 -w -std=c99
TARGET=zacastle
all: $(TARGET)

$(TARGET): zacastle.c
	$(CC) zacastle.c $(CFLAGS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
