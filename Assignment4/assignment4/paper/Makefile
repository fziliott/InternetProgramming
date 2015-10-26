CC = gcc
CFLAGS =  -g
LDFLAGS = 
SOURCES_C = paperserver.c paperclient.c papers.c paperinfo.c paperload.c paperview.c
OBJECTS = paperclient.o paperserver_clnt.o paperserver_xdr.o paperserver.o paperserver_svc.o add_clnt.c  papers.o paperinfo.o paperview.o paperload.o
SOURCES_JAVA = ./client/HotelClient.java ./server/HotelServer.java
EXEC = paperserver paperclient papers.cgi paperinfo.cgi  

build: $(EXEC)

jrun: java run

all: clean build

papers.cgi: papers.o
		$(CC) -o papers papers.o paperserver_clnt.o paperserver_xdr.o  -lnsl  
paperinfo.cgi: paperinfo.o
		$(CC) -o paperinfo.cgi paperinfo.o paperserver_clnt.o paperserver_xdr.o  -lnsl 
paperload.cgi: paperload.o
		$(CC) -o paperload paperload.o paperserver_clnt.o paperserver_xdr.o  -lnsl  
paperview.cgi: paperview.o
		$(CC) -o paperview paperview.o paperserver_clnt.o paperserver_xdr.o  -lnsl 
papers.o: papers.c paperserver.h 
		$(CC) $(CFLAGS) -c -o papers.o papers.c
paperinfo.o: paperinfo.c paperserver.h
		$(CC) $(CFLAGS) -c -o paperinfo.o paperinfo.c
paperload.o: paperload.c paperserver.h
		$(CC) $(CFLAGS) -c -o paperload.o paperload.c
paperview.o: paperview.c paperserver.h
		$(CC) $(CFLAGS) -c -o paperview.o paperview.c


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
