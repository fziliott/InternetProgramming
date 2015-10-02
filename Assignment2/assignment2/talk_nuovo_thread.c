#include <pthread.h>
#include <curses.h>
#include <termios.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define BACKLOG 5
#define PORT 5555

int done = 0;
int sockfd;

WINDOW *receive_win;
WINDOW *send_win;
int receive_win_line = 1;
int send_win_line = 1;
int width, height;

void *send_message();

void *get_message();

void handler() {
    close(sockfd);
    wclear(receive_win);
    wclear(send_win);
    exit(0);
}

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if  ((nwritten = write(fd, ptr, nleft)) <= 0 ) {
            if (errno == EINTR)
                nwritten = 0; /* and call write() again */
            else
                return -1; /* error */
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}


int openSocket(struct sockaddr_in *server_addr) {
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    int enable = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }

    if(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd, (struct sockaddr *) server_addr,
             sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
    }

    if (listen(fd, BACKLOG) == -1)
        perror("listen");

    return fd;
}

int main(int argc, char **argv) {

    /*windows setting*/
    initscr();
    getmaxyx(stdscr, height, width);
    receive_win = newwin(height / 2, width, 0, 0);
    send_win = newwin(height / 2, width, height / 2, 0);
    //whline(send_win, 1,width);
    wborder(receive_win,' ',' ',' ','_',' ',' ',' ',' ');
    scrollok(receive_win, TRUE);
    scrollok(send_win, TRUE);
    wsetscrreg(receive_win, 1, height / 2 - 2);
    wsetscrreg(send_win, 1, height / 2 - 2);
    wrefresh(receive_win);
    wrefresh(send_win);

    /*client*/
    if(argc == 2) {
        struct sockaddr_in remote;
        struct hostent *host;

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        host = gethostbyname(argv[1]);
        if (host == NULL) {
            fprintf(stderr, "Hostname \"%s\" could not be resolved\n", argv[1]);
            exit(-2);
        }
        char *addr[5];
        int i = 0;
        struct in_addr **q = (struct in_addr **)host->h_addr_list;
        while (*q != NULL) {
            addr[i] = malloc(16 * sizeof(char));
            strcpy(addr[i], inet_ntoa(**q));
            q++;
            ++i;
        }

        remote.sin_family = AF_INET;
        remote.sin_port = htons(PORT);
        remote.sin_addr.s_addr = inet_addr(addr[0]);
        printf("Addr: %s, PORT: %d\n", addr[0], PORT);
        if (connect(sockfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in)) == -1) {
            perror("connect");
            exit(1);
        }
    } else {    /*server*/
        int num;
        struct sockaddr_in server_addr, client_addr;;
        sockfd = openSocket(&server_addr);
        int yes = 1;
        /*ignore address already in use'*/
        if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) {
            perror("setsockopt");
        }
        /*ignore port already in use'*/
        if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1 ) {
            perror("setsockopt");
        }
        int addrlen = sizeof(struct sockaddr_in);
        sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &addrlen);
        //printf("Request\n");
        if(sockfd < 0) {
            handler();
        }
    }

    signal(SIGINT, handler);

    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, send_message, NULL);
    pthread_create(&threads[1], &attr, get_message, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

}


void *send_message() {

    char message[512];

    while(1) {
        bzero(message, 512);
        wrefresh(receive_win);
        wrefresh(send_win);

        mvwgetstr(send_win, send_win_line, 2, message);
        writen(sockfd, message, strlen(message) + 1);

        if(send_win_line < height / 2 - 2)
            send_win_line++;
        else scroll(send_win);

    }
}


void *get_message() {
    char message[512];

    while(1) {
        bzero(message, 512);
        wrefresh(receive_win);
        wrefresh(send_win);

        if((recv(sockfd, message, 512, 0)) <= 0) {
            handler();
            exit(1);
        }

        mvwprintw(receive_win, receive_win_line, 2, message);

        if(receive_win_line < height / 2 - 2)
            receive_win_line++;
        else scroll(receive_win);


    }
}
