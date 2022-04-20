CC=gcc
CFLAGS=-O3 -w -std=c99
TARGET=zacastle
#KEYGEN=castle-keygen
all: $(TARGET)
#all: $(TARGET) $(KEYGEN)

$(TARGET): zacastle.c
	$(CC) zacastle.c $(CFLAGS) -o $(TARGET)

#$(KEYGEN): qloq_keygen.c
#	$(CC) qloq_keygen.c $(CFLAGS) -o $(KEYGEN)

#clean:
#	$(RM) $(TARGET) $(RM) $(KEYGEN)

clean:
	$(RM) $(TARGET)
