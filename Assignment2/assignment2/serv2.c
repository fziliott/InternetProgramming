#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define BACKLOG 5
#define PORT 4444

int counter = 0;

struct sembuf up =   {0, 1, 0}; //struct for the UP() operation on semaphore
struct sembuf down = {0, -1, 0};//struct for the DOWN() operation on semaphore

void sig_chld(int a);

ssize_t writen(int fd, const void *vptr, size_t n);

int openSocket(struct sockaddr_in *server_addr);

void treat_request(int socketfd, int sem, int *counter);

int main(void) {
    int *counter;
    int socketfd, newsfd;
    struct sockaddr_in server_addr, client_addr;


    int sem = semget(IPC_PRIVATE, 1, 0600);     //semaphore
    semop(sem, &up, 1);     //at the begin the semaphore must be set to 1 (open)
    int id = shmget(IPC_PRIVATE, sizeof(int), 0600 | IPC_CREAT);
    if (id < 0) {
        perror("Error shmget");
        exit(1);
    }
    counter = (int *) shmat(id, 0, 0);
    *counter = 0;

    socketfd = openSocket(&server_addr);

    signal(SIGCHLD, sig_chld);
    while (1) {
        int addrlen = sizeof(struct sockaddr_in);
        newsfd = accept(socketfd, (struct sockaddr *) &client_addr, &addrlen);
        if(newsfd < 0)
            continue;
        if (fork() == 0) {
            treat_request(newsfd, sem, counter);
            exit(0);
        } else {
            close(newsfd);
        }
    }
}


void treat_request(int socketfd, int sem, int *counter) {
    semop(sem, &down, 0);
    (*counter)++;
    uint32_t msg = htonl(*counter);
    semop(sem, &up, 0);

    if (writen(socketfd, (void *)&msg, sizeof(int)) == -1) {
        int i;
        perror("send");
    }
}


int openSocket(struct sockaddr_in *server_addr) {
    int socketfd;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    int enable = 1;
    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }
     if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(PORT);
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketfd, (struct sockaddr *) server_addr,
             sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
    }

    if (listen(socketfd, BACKLOG) == -1)
        perror("listen");

    return socketfd;
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


void sig_chld(int a) {
    while (waitpid(0, NULL, WNOHANG) > 0) {}
    signal(SIGCHLD, sig_chld);
}