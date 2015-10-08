CC = gcc
CFLAGS =  -g
LDFLAGS = 
SOURCES_C = paperserver.c paperclient.c
SOURCES_JAVA = ./client/HotelClient.java ./server/HotelServer.java
EXEC = paperserver paperclient

build: $(EXEC) 

jrun: java run

all: clean build 

paperclient: paperclient.o storage_clnt.o storage_xdr.o 
	$(CC) -o paperclient paperclient.o storage_clnt.o storage_xdr.o -lnsl  

paperserver: paperserver.o add_svc.o add_xdr.o
	$(CC) -o paperserver paperserver.o storage_svc.o storage_xdr.o -lnsl 

add_clnt.c storage.h storage_xdr.c storage_svc.c: storage.x
	rpcgen storage.x

paperclient.o: paperclient.c storage.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

add_clnt.o: storage_clnt.c storage.h
	$(CC) $(CFLAGS) -c -o storage_clnt.o storage_clnt.c

add_xdr.o: storage_xdr.c storage.h
	$(CC) $(CFLAGS) -c -o storage_xdr.o storage_xdr.c

add_svc.o: storage_svc.c storage.h
	$(CC) $(CFLAGS) -c -o storage_svc.o storage_svc.c

paperserver.o: paperserver.c storage.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c

java: $(SOURCES_JAVA)
		javac -d ./server HotelServer.java
		javac -d ./client HotelClient.java

run:
		rmiregistry &
		java -cp ./server HotelServer &
		java -cp ./client HotelClient

clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.out

.PHONY:  paperserver paperclient clean all compile