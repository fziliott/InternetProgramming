CC = gcc
CFLAGS =  -g
LDFLAGS = 
SOURCES_C = paperserver.c paperclient.c papers.c paperinfo.c paperload.c paperview.c
OBJECTS = paperclient.o paperserver_clnt.o paperserver_xdr.o paperserver.o paperserver_svc.o add_clnt.c  papers.o paperinfo.o paperview.o paperload.o
SOURCES_JAVA = ./client/HotelClient.java ./server/HotelServer.java
EXEC = paperserver paperclient

build: $(EXEC)

jrun: java run

all: clean build


paperclient: paperclient.o paperserver_clnt.o paperserver_xdr.o 
	$(CC) -o paperclient paperclient.o paperserver_clnt.o paperserver_xdr.o -lnsl  

paperserver: paperserver.o paperserver_svc.o paperserver_xdr.o
	$(CC) -o paperserver paperserver.o paperserver_svc.o paperserver_xdr.o -lnsl 

add_clnt.c paperserver.h paperserver_xdr.c paperserver_svc.c: paperserver.x
	rpcgen paperserver.x

paperclient.o: paperclient.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperclient.o paperclient.c

paperserver_clnt.o: paperserver_clnt.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_clnt.o paperserver_clnt.c

paperserver_xdr.o: paperserver_xdr.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_xdr.o paperserver_xdr.c

paperserver_svc.o: paperserver_svc.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver_svc.o paperserver_svc.c

paperserver.o: paperserver.c paperserver.h
	$(CC) $(CFLAGS) -c -o paperserver.o paperserver.c

clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.out paperserver.h paperserver_clnt.c  paperserver_xdr.c paperserver_svc.c

.PHONY:  paperserver paperclient papers paperinfo paperload paperview clean all compile
