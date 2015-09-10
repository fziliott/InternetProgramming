CC = gcc
CFLAGS = 
TARGET_SOURCES = mysh1.c  syn1.c syn2.c
TARGET_OBJECTS = mysh1.o syn1.o syn2.o
EXECUTABLE = mysh1 syn1 syn2

all: $(EXECUTABLE)

compile: $(TARGET_OBJECTS)

$(EXECUTABLE): $(TARGET_OBJECTS)
		$(CC) $(CFLAGS) $@.c -o $@

$(TARGET_OBJECTS): $(TARGET_SOURCES)
		$(CC) $(CFLAGS) -c $^

clean:
		rm -f *.o $(EXECUTABLE)

.PHONY: clean all compile
