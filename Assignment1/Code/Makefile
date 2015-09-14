CC = gcc
CFLAGS = –pthread
JAVAC = javac
JFLAGS =
TARGET_SOURCES = mysh1.c  syn1.c syn2.c synthread1.c synthread2.c
TARGET_OBJECTS = mysh1.o syn1.o syn2.o synthread1.o synthread2.o syn1.class syn2.class
EXECUTABLE = mysh1 syn1 syn2 synthread1 synthread2

all: $(CEXECUTABLE) $(JAVAEX)

compile: $(TARGET_OBJECTS)

$(EXECUTABLE): $(TARGET_OBJECTS)
		$(CC) $(CFLAGS) $@.c -o $@


$(TARGET_OBJECTS): $(TARGET_SOURCES)
		$(CC) $(CFLAGS) -c $^
		 
jav: syn1.java syn2.java
		javac syn1.java
		javac syn2.java

clean:
		rm -f *.o $(EXECUTABLE)

.PHONY: clean all compile
