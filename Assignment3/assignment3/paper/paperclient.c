#include "paperserver.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler();
void setSignals();

retrieved_article *ra;
article_list *al;
article_info *ai;
sent_article *sa;

int main(int argc, char **argv) {
    setSignals();
    CLIENT *cl;
    article_request ar;

    char author[MAXLEN];
    char name[MAXLEN];
    int *id;
    char *cvalue;
    int c;
    int num_read = 0;
    FILE* file;
 
    cl = clnt_create(argv[1], ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        return 1;
    }

    while ((c = getopt (argc, argv, "ha:f:i:r:l")) != -1) {
        switch (c) {
        case 'h':
            printf("List of available commands:\n");
            printf("\t-h see help manual\n\n");
            printf("\t-l list of all the papers with 'author' and 'filename'\n\n");
            printf("\t-a <author> <filename> <file>: add a paper to the collection. <'author'> is the author name, <'filename'> is the name with which the new paper will be saved and <file> is the name of the local file that contains the paper to send. If the <author>-<filename> couple is already present, the corresponding paper will be updated.\n\n");
            printf("\t-f <id>: retrieves the paper number <id> and prints it to the standard output. If the paper is not found an error message is prompted.\n\n");
            printf("\t-i <id>: get the information about paper <id> (author name and file name) and prints them to the standard output.\n\n");
            printf("\t-r <id>: removes the article with code <id> from the collection.\n\n");
            break;

        case 'a':
            strcpy(author, argv[optind - 1]);
            strcpy(name, argv[optind]);

            file = fopen(argv[optind + 1], "rb");
            if (file == NULL) {
                return -1;
            }
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            rewind(file);
            sa = malloc(sizeof(struct sent_article));
            sa->start = 0;
            sa->finish = 1;

            sa->author = malloc(strlen(author));
            strcpy(sa->author, author);
            sa->name = malloc(strlen(name));
            strcpy(sa->name, name);
            sa->data.data_val = malloc(size);
            num_read = fread(sa->data.data_val, 1, size, file);
            sa->data.data_len = num_read;
            id = sendarticle_1(sa, cl);
            free(sa->data.data_val);
            free(sa);

            if(id != NULL && *id != -1) {
                printf("%d\n", *id);
            } else {
                printf("Can't store your article\n");
            }
            break;

        case 'f':
            cvalue = optarg;
            int read = 0;

            ar.articleID = atoi(cvalue);
            ar.start = 0;
            ra = retrievearticle_1(&ar, cl);
            int n = fwrite(ra->data.data_val, sizeof(char), ra->data.data_len, stdout);

            fflush(stdout);
            break;

        case 'i':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            ai = retrievearticleinfo_1(&ar, cl);
            if(ai == NULL) {
                break;
            }
            if(ai->id == -1) {
                printf("No article with id: %d\n", ar.articleID);
            } else {
                if(ai != NULL) {
                    printf("%s\t%s\n", ai->author, ai->name);
                } else {
                    printf("Can't retrieve article info\n");
                }
            }
            break;


        case 'r':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            int *res;
            res = removearticle_1(&ar, cl);
            if(res != NULL) {
                if (*res == 0)
                    printf("paper removed\n");
                else
                    printf("can't find or remove paper\n");
            }
            break;

        case 'l':
            al = listarticle_1(NULL, cl);
            while(al != NULL && al->item != NULL && al->item->id != -1) {
                printf( "%d\t%s\t%s\n", al->item->id, al->item->author, al->item->name);
                al = al->next;
            }
            break;

        default:
            abort();
            break;
        }
    }
    return 0;
}

void handler() {
    if(sa!=NULL){
        if(sa->data.data_val!= NULL)
            free(sa->data.data_val);
        free(sa);
    }
    sa=NULL;
    exit(0);
}

void setSignals(){
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGQUIT, handler);
    signal(SIGHUP, handler);
}
