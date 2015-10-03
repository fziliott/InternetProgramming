#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>


#define BACKLOG 5
#define PORT 4444

int counter=0;
int sfd;

/*finalization function, for a safe closure*/
void handler(){
    close(sfd);
    exit(0);
}

/*function to assure the send of all needed bytes*/
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

/*manage a new connection*/
void treat_request(int socket) {
    counter++;
    uint32_t msg=htonl(counter);
    if (writen(socket, (void *)&msg, sizeof(int)) == -1) {
        int i;
        perror("send");
    }
}


int main(void) {
    int newsfd;
    struct sockaddr_in server_addr, client_addr;

    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    signal(SIGINT, handler);
    int enable = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }
     if(setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0) {
        perror("Error setting socket options");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *) &server_addr,
             sizeof(struct sockaddr_in)) == -1) {
        perror("bind");
    }

    if (listen(sfd, BACKLOG) == -1)
        perror("listen");

    while (1) {
        int addrlen = sizeof(struct sockaddr_in);
        newsfd = accept(sfd, (struct sockaddr *) &client_addr, &addrlen);
        if(newsfd < 0)
            continue;
        treat_request(newsfd);
        close(newsfd);
    }

    close(sfd);

    return 0;
}
