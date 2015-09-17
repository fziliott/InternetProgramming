CC = gcc
CFLAGS =  
LDFLAGS = -lpthread
SOURCES_C = mysh1.c mysh2.c mysh3.c syn1.c syn2.c synthread1.c synthread2.c 
SOURCES_JAVA = syn1.java syn2.java
OBJECTS=$(SOURCES_C:.c=.o)
EXEC = mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2
CLASSES=$(SOURCES_JAVA:.java=.class)

all: c java

c: $(EXEC)

$(EXEC): $(OBJECTS)
		$(CC) -o $@ $< $(LDFLAGS)

$(OBJECTS): $(SOURCES_C)
		$(CC) $(CFLAGS) -c $^

java: $(SOURCES_JAVA)
		javac $^

clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.class *.out

.PHONY: clean all compile