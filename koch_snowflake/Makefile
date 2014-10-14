CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=-lSDL -lm
SOURCES=koch_snowflake.c list.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=koch_snowflake.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
