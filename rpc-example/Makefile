CC     := gcc
CFLAGS := -Wall 

all: client server

clean:
	rm -f client.o add_clnt.o add_xdr.o add_svc.o serverproc.o add_clnt.c add.h add_xdr.c add_svc.c client server

client: client.o add_clnt.o add_xdr.o 
	$(CC) -o client client.o add_clnt.o add_xdr.o -lnsl  

server: serverproc.o add_svc.o add_xdr.o
	$(CC) -o server serverproc.o add_svc.o add_xdr.o -lnsl 

add_clnt.c add.h add_xdr.c add_svc.c: add.x
	rpcgen add.x

client.o: client.c add.h
	$(CC) $(CFLAGS) -c -o client.o client.c

add_clnt.o: add_clnt.c add.h
	$(CC) $(CFLAGS) -c -o add_clnt.o add_clnt.c

add_xdr.o: add_xdr.c add.h
	$(CC) $(CFLAGS) -c -o add_xdr.o add_xdr.c

add_svc.o: add_svc.c add.h
	$(CC) $(CFLAGS) -c -o add_svc.o add_svc.c

serverproc.o: serverproc.c add.h
	$(CC) $(CFLAGS) -c -o serverproc.o serverproc.c

