# Simple Makefile for compiling transporte.c

# Target executable name
TARGET=transporte

all:
	gcc transporte.c -o $(TARGET)

clean:
	rm -f $(TARGET)
