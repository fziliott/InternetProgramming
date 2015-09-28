/*to compile  gcc -o ncurses ncurses.c -lncurses*/

#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define BACKLOG 5
#define PORT 5553
#define NB_PROC 10

int *counter;
int socketfd;
WINDOW *send_win;		//fist window (sent messages)
WINDOW *recieve_win;	//second window (recieved messages)
int input = 1;			//line number for sent messages window
int line = 1;			//line number for recieved messages window
int max_x, max_y;


void sig_chld(int a);
void handler() {
    close(socketfd);
    delwin(send_win);
    delwin(recieve_win);
    endwin();
    exit(1);
}

/*void do_resize() {
	int new_x,new_y,max_x,max_y;
    getmaxyx(stdscr, new_y, new_x);
    if (new_y != max_y || new_x != max_x) {
        max_x = new_x;
        max_y = new_y;
        wresize(send_win, new_y - new_y / 2, new_x);
        wresize(recieve_win, new_y / 2, new_x);
        mvwin(recieve_win, new_y - new_y / 2, 0);
        wclear(stdscr);
        wclear(send_win);
        wclear(recieve_win);
        draw_borders(send_win);
        draw_borders(recieve_win);

    }*/

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;
    char msg[100];
    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if  ((nwritten = write(fd, ptr, nleft)) <= 0 ) {
            //sprintf(msg, "%zu", nwritten);
            mvwprintw(send_win, line, 2, msg);
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

void treat_request_read() {
    char leggi[500];
    char msg[600];
    //wtimeout(recieve_win, -1);
    while(1) {
        bzero(leggi, 500);
        bzero(msg, 600);
        wrefresh(recieve_win);
        wrefresh(send_win);
        if((recv(socketfd, leggi, 500, 0)) > 0) {
            strcpy(msg, "Other: ");
            strcat(msg, leggi);
            mvwprintw(recieve_win, line, 2, msg);
            //if(line != max_y / 2 - 2)
                line++;
            //else
             //   scroll(send_win);
        } else {
            handler();
            perror("read");
            exit(1);
        }
    }
}

void treat_request_write() {
    char str[80];
    char msg[100];
    wtimeout(send_win, -1);	//to read from stdin now is blocking
    while(1) {
        bzero(str, 80);
        bzero(msg, 100);
        wrefresh(recieve_win);
        wrefresh(send_win);
        mvwgetstr(send_win, input, 2, str);
        writen(socketfd, (void *)str, strlen(str)+1);
        strcpy(msg, "Me:");
        strcat(msg, str);
        mvwprintw(send_win, input, 2, msg);
        wrefresh(send_win);
        wrefresh(recieve_win);

        //if(input != max_y / 2 - 2)
            input++;
        //else
            //scroll(recieve_win);
    }
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


void sig_chld(int a) {
    while (waitpid(0, NULL, WNOHANG) > 0) {}
    signal(SIGCHLD, sig_chld);
}

int main(int argc, char *argv[]) {
    initscr();
    //noecho();
    //curs_set(FALSE);
    // get our maximum window dimensions
    getmaxyx(stdscr, max_y, max_x);

    // set up initial windows
    /*WINDOW *field = newwin(max_y - score_size, max_x, 0, 0);
    WINDOW *score = newwin(score_size, max_x, max_y - score_size, 0);*/
    send_win = newwin(max_y / 2, max_x, 0, 0);
    recieve_win = newwin(max_y / 2, max_x, max_y - max_y / 2, 0);

    /*scrollok(send_win, TRUE);
    scrollok(recieve_win, TRUE);*/
    box(send_win, '|', '=');
    box(recieve_win, '|', '-');

    /*wsetscrreg(send_win, 1, max_y / 2 - 2);
    wsetscrreg(recieve_win, 1, max_y / 2 - 2);*/



    wtimeout(send_win, -1); //blocking reads
    //wtimeout(recieve_win, -1); //blocking reads

    /*// draw to our windows
      mvwprintw(send_win, 0, 0, "send_win");
      mvwprintw(recieve_win, 0, 0, "recieve_win");
      // refresh each window
      wrefresh(send_win);
      wrefresh(recieve_win);
      sleep(5);
      // clean up
      delwin(send_win);
      delwin(recieve_win);
      endwin();*/

    // draw our borders
    /*draw_borders(send_win);
    draw_borders(recieve_win);*/
    // simulate the game loop
    /* while(1) {
       // draw to our windows
       mvwprintw(send_win, 1, 1, "Text typed locally:");
       mvwprintw(recieve_win, 1, 1, "Text typed remotely:");
       // refresh each window
       wrefresh(send_win);
       wrefresh(recieve_win);
     }
     // clean up
     delwin(send_win);
     delwin(recieve_win);
     // ...

    */
    //signal(SIDWINCH, do_resize);

    wrefresh(send_win);
    wrefresh(recieve_win);



    struct hostent *host;
    if(argc < 2) {
        printf("Couldn't get server name!\n");
        exit(1);
    }
    struct sockaddr_in remote;

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
    //printf("Addr: %s, PORT: %d\n", addr[0], PORT);
    if (connect(socketfd, (struct sockaddr *)&remote, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(1);
    }

    signal(SIGINT, handler);

    if (fork() == 0) {
        treat_request_write(socketfd);
    } else treat_request_read(socketfd);


    // write it in chat window (top)
    // mvwprintw(top,line,2,msg);


    /*
      while(1) {
        getmaxyx(stdscr, new_y, new_x);
        if (new_y != max_y || new_x != max_x) {
          max_x = new_x;
          max_y = new_y;
          wresize(send_win, new_y - new_y/2, new_x);
          wresize(recieve_win, new_y/2, new_x);
          mvwin(recieve_win, new_y - new_y/2, 0);
          wclear(stdscr);
          wclear(send_win);
          wclear(recieve_win);
          draw_borders(send_win);
          draw_borders(recieve_win);

        }
    */
    // draw to our windows
    /* mvwprintw(send_win, 1, 1, "Text typed locally.");
     mvwprintw(recieve_win, 1, 1, "Text typed remotely.");
     // refresh each window
     wrefresh(send_win);
     wrefresh(recieve_win);*/





    return 0;
}
