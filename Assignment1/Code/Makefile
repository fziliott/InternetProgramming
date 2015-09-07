CC = gcc
CFLAGS = -Wall 
TARGET_SOURCES = syn1.c syn2.c
TARGET_OBJECTS = syn1.o syn2.o
EXECUTABLE = syn1 syn2

all: $(EXECUTABLE)

compile: $(TARGET_OBJECTS)

$(EXECUTABLE): $(TARGET_OBJECTS)
		$(CC) $(CFLAGS) $@.c -o $@

$(TARGET_OBJECTS): $(TARGET_SOURCES)
		$(CC) $(CFLAGS) -c $^

clean:
		rm -f *.o $(EXECUTABLE)

.PHONY: clean all compile