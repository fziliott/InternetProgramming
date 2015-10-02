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

//char name[20];

int sockfd;
WINDOW *recieve_win;
WINDOW *send_win;
int line = 1; // Line position of recieve_win
int input = 1; // Line position of recieve_win
int maxx, maxy; // Screen dimensions
//pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
int mutex;
struct sembuf up = {0, 1, 0};
struct sembuf down = {1, 1, 0};

void *sendmessage(void *name);

void *listener();

void handler() {
    close(sockfd);
    endwin();
    //pthread_mutex_destroy(&mutexsum);
    exit(1);
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
    mutex = semget(IPC_PRIVATE, 1, 600);
    semop(mutex, &up, 1);

    char name[] = "Fabrizio";

    // Set up windows
    initscr();
    getmaxyx(stdscr, maxy, maxx);

    recieve_win = newwin(maxy / 2, maxx, 0, 0);
    send_win = newwin(maxy / 2, maxx, maxy / 2, 0);

    scrollok(recieve_win, TRUE);
    scrollok(send_win, TRUE);
    box(recieve_win, '|', '=');
    box(send_win, '|', '-');

    wsetscrreg(recieve_win, 1, maxy / 2 - 2);
    wsetscrreg(send_win, 1, maxy / 2 - 2);
    wrefresh(recieve_win);
    wrefresh(send_win);


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
    } else {
        int num;
        struct sockaddr_in server_addr, client_addr;;
        sockfd = openSocket(&server_addr);
        signal(SIGINT, handler);
        int yes = 1;
        /*ignore 'address already in use'*/
        if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) {
            perror("setsockopt");
        }
        if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) == -1 ) {
            perror("setsockopt");
        }
        int addrlen = sizeof(struct sockaddr_in);
        sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &addrlen);
        printf("Request\n");
        if(sockfd < 0)
            exit(1);
    }

    int len;
    int result;
    char buf[256];

    // Set up threads
    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Spawn the listen/receive deamons
    pthread_create(&threads[0], &attr, sendmessage, (void *)name);
    pthread_create(&threads[1], &attr, listener, NULL);
    //pthread_join();

    // Keep alive until finish condition is done
    //while(!done);

    /*if (fork() == 0) {
        sendmessage(name);
    } else listener();*/
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

}


void *sendmessage(void *name) {

    char str[80];
    char msg[100];
    int bufsize = maxx - 4;
    char *buffer = malloc(bufsize);

    while(1) {
        //bzero(str, 80);
        //bzero(msg, 100);
        //bzero(buffer, bufsize);
        wrefresh(recieve_win);
        wrefresh(send_win);

        // Get user's message
        mvwgetstr(send_win, input, 2, str);

        // Build the message: "name: message"
        strcpy(msg, name);
        strncat(msg, ": \0", 100 - strlen(str));
        strncat(msg, str, 100 - strlen(str));

        /*// Check for quiting
        if(strcmp(str, "exit") == 0) {

            done = 1;

            // Clean up
            endwin();
            //pthread_mutex_destroy(&mutexsum);
            //pthread_exit(NULL);
            close(sockfd);
        }*/

        // Send message to server
        writen(sockfd, msg, strlen(msg));

        // write it in chat window (recieve_win)
        //mvwprintw(recieve_win, line, 2, msg);


        // scroll the recieve_win if the line number exceed height
        //pthread_mutex_lock (&mutexsum);
        /*semop(mutex,&down,1);
        if(line != maxy / 2 - 2)
            line++;
        else
            scroll(recieve_win);*/

        // scroll the send_win if the line number exceed height
        if(input != maxy / 2 - 2)
            input++;
        else
            scroll(send_win);
        //semop(mutex,&up,1);

        //pthread_mutex_unlock (&mutexsum);
    }
}


// Listen for messages and display them
void *listener() {
    char str[80];
    int bufsize = maxx - 4;
    char *buffer = malloc(bufsize);

    while(1) {
        bzero(buffer, bufsize);
        wrefresh(recieve_win);
        wrefresh(send_win);

        //Receive message from server
        if((recv(sockfd, buffer, bufsize, 0)) <= 0) {
            handler();
            exit(1);
        }

        //read(sockfd, buffer, bufsize);

        //Print on own terminal
        mvwprintw(recieve_win, line, 2, buffer);

        // scroll the recieve_win if the line number exceed height
        //pthread_mutex_lock (&mutexsum);
        //semop(mutex, &down, 1);
        if(line != maxy / 2 - 2)
            line++;
        else
            scroll(recieve_win);
        //pthread_mutex_unlock (&mutexsum);
        //semop(mutex, &up, 1);
    }
}
