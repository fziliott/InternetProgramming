CC = gcc
CFLAGS = -Wall 
LDFLAGS = -lpthread
SOURCES_C = mysh1.c mysh2.c mysh3.c syn1.c syn2.c synthread1.c synthread2.c 
SOURCES_JAVA = syn1.java syn2.java
OBJECTS=$(SOURCES_C:.c=.o)
CLASSES=$(SOURCES_JAVA:.java=.class)
EXECUTABLE = mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2

all: c java

compile: $(OBJECTS)

c: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		$(CC) -o $@ $< $(LDFLAGS)

$(TARGET_OBJECTS): $(SOURCES_C)
		$(CC) $(CFLAGS) -c $^

java: $(SOURCES_JAVA)
		javac $^

clean:
		rm -rf $(OBJECTS) $(EXECUTABLE:.$(EXT)=.o) $(CLASSES)

.PHONY: clean all compile