CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lm -lSDL
SOURCES=complex.c mandelbrot.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mandelbrot

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
