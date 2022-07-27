CC=gcc
CFLAGS=-O3 -w -std=c99 -ggdb
TARGET=zacastle
all: $(TARGET)

$(TARGET): zacastle.c
	$(CC) zacastle.c $(CFLAGS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
