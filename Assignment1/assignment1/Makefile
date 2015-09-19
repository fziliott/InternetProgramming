CC = gcc
CFLAGS =  
LDFLAGS = -lpthread
SOURCES_C = mysh1.c mysh2.c mysh3.c syn1.c syn2.c synthread1.c synthread2.c 
SOURCES_JAVA = syn1.java syn2.java
OBJECTS=$(SOURCES_C:.c=.o)
EXEC = mysh1 mysh2 mysh3 syn1 syn2 synthread1 synthread2
CLASSES=$(SOURCES_JAVA:.java=.class)

build: all
	
all: $(EXEC) java

mysh1: 
		gcc -o mysh1 mysh1.c
mysh2: 
		gcc -o mysh2 mysh2.c
mysh3: 
		gcc -o mysh3 mysh3.c
synthread1: 
		gcc -o synthread1 synthread1.c -lpthread
synthread2: 
		gcc -o synthread1 synthread2.c -lpthread
syn1: 
		gcc -o syn1 syn1.c
syn2: 
		gcc -o syn2 syn2.c

java: $(SOURCES_JAVA)
		javac $^

clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.class *.out

.PHONY: clean all compile