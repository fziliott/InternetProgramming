CC = cc
CFLAGS =  -g
LDFLAGS = ../../ccgi/libccgi.a
SOURCES_C = paperserver.c
OBJECTS = papers.o paperload.o paperinfo.o paperview.o
EXEC = papers.cgi paperinfo.cgi paperload.cgi paperview.cgi
INCLUDE = ../../paper/paperserver_clnt.o ../../paper/paperserver_xdr.o

build: $(EXEC) 
	make -C ../../paper

all: clean build 

papers.cgi:
		$(CC) -o papers.cgi papers.c  $(INCLUDE) -lnsl   $(LDFLAGS)

paperinfo.cgi:
		$(CC) -o paperinfo.cgi paperinfo.c  $(INCLUDE) -lnsl   $(LDFLAGS)

paperload.cgi:
		$(CC) -o paperload.cgi paperload.c  $(INCLUDE) -lnsl   $(LDFLAGS)

paperview.cgi:
		$(CC) -o paperview.cgi paperview.c  $(INCLUDE) -lnsl   $(LDFLAGS)


clean:
		rm -rf $(OBJECTS) $(EXEC:.$(EXT)=.o) *.out 

.PHONY:  papers.cgi clean all compile
