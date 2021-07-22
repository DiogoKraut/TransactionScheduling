# Diogo Paris Kraut - GRR 20166365

TARGET = escalona
CC = gcc
CFLAGS = -Wall -g -Iheader

SRCDIR = src/
LIBDIR = header/
OBJECTS = $(patsubst %.c, %.o, $(wildcard $(SRCDIR)*.c))
HEADERS = $(wildcard $(LIBDIR)*.h)

default: $(TARGET)
all: default

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

clean:
	-rm -f *.o core

purge:
	-rm -f src/*.o
	-rm -f *.o core
	-rm -f $(TARGET)