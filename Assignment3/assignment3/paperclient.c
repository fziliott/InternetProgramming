#include "storage.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    CLIENT *cl;
    article_request ar;
    retrieved_article *ra;
    article_list *al;
    article_info *ai;
    int *id;
    sent_article sa;
    char *cvalue;
    FILE *file;
    int c;

    cl = clnt_create(argv[1], ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        return 1;
    }

    while ((c = getopt (argc, argv, "ha:f:i:r:l")) != -1) switch (c) {
        case
                'h':
            printf("List of available commands:\n");
            printf("\t-h see help manual\n\n");
            printf("\t-l list of all the papers with 'author' and 'filename'\n\n");
            printf("\t-a <author> <filename> <file>: add a paper to the collection. <'author'> is the author name, <'filename'> is the name with which the new paper will be saved and <file> is the name of the local file that contains the paper to send. If the <author>-<filename> couple is already present, the corresponding paper will be updated.\n\n");
            printf("\t-f <id>: retrieves the paper number <id> and prints it to the standard output. If the paper is not found an error message is prompted.\n\n");
            printf("\t-i <id>: get the information about paper <id> (author name and file name) and prints them to the standard output.\n\n");
            printf("\t-r <id>: removes the article with code <id> from the collection.\n\n");
            break;

        case 'a':
            //bzero(sa.data, MAXLEN);
            printf("index  %s\n", argv[optind]);

            strcpy(sa.author, argv[optind - 1]);
            printf("%s\n", argv[optind] );
            strcpy(sa.name, argv[optind]);
            printf("name %s, author:%s nomefile:%s\n", sa.name, sa.author, argv[optind + 1]);



            file = fopen(argv[optind + 1], "rb");
            if (file == NULL) {
                return -1;
            }
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            printf("prima di rewind\n");
            rewind(file);
            printf("prima di malloc\n");
            sa.data = malloc(sizeof(char) * size);
            printf("prima di read\n");
            int n = fread(sa.data, 1, sizeof(char) * size, file);
            sa.size = n;

            printf("data: %s, length: %d", sa.data, n);
            id = sendarticle_1(&sa, cl);
            if(id != NULL && *id != -1) {
                printf("%d\n", *id);
            } else {
                printf("Can't store your article\n");
            }
            break;

        case 'f':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            ra = retrievearticle_1(&ar, cl);
            if(ra == NULL) {
                printf("Can't find the article %d\n", ar.articleID);
            } else {
                fwrite(ra->data, ra->size, 1, stdout);
            }
            break;

        case 'i':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            ai = retrievearticleinfo_1(&ar, cl);
            if(ai != NULL) {

                printf("%s\t%s\n", ai->author, ai->name);
            } else {
                printf("Can't retrieve article info\n");
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
            while(al != NULL ) {
                printf( "%d\t%s\t%s\n", al->item->id, al->item->author, al->item->name);
                al = al->next;
            }
            break;

        default:
            abort ();
        }
    return 0;
}
