CC = gcc
CFLAGS =  
LDFLAGS = -lpthread
SOURCES_C = serv1.c serv2.c serv3.c client.c talk.c
EXEC = serv1 serv2 serv3 client  talk

build: $(EXEC) 

all: clean build

serv1: 
		gcc -o serv1 serv1.c 
serv2: 
		gcc -o serv2 serv2.c
serv3: 
		gcc -o serv3 serv3.c
client: 
		gcc -o client client.c
talk: 
		gcc -o talk talk.c -lpthread -lncurses


clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.out

.PHONY: clean all compile