#include "storage.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    CLIENT *cl;
    article_request ar;
    retrieved_article *ra;
    int *id;
    sent_article sa;
    char arguments[1024];
    char *cvalue;
    char *token;
    FILE *file;
    int c;

    cl = clnt_create(argv[1], ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        return 1;
    }

    while ((c = getopt (argc, argv, "ha:f:i:r:l")) != -1)
        switch (c) {
        case 'h':
            break;
        case 'a':
            bzero(sa.data, MAXLEN);
            printf("index  %s\n", argv[optind]);

            strcpy(sa.author, argv[optind - 1]);
            printf("%s\n", argv[optind] );
            strcpy(sa.name, argv[optind]);
            printf("name %s, author:%s nomefile:%s\n", sa.name, sa.author, argv[optind + 1]);


            file = fopen(argv[optind + 1], "rb");
            if (file == NULL) {
                printf("NULL");
                return -1;
            }

            int n = fread(sa.data, 1, MAXLEN2, file);
            sa.size=n;
            //sa.data[n] = EOF; //why?




            printf("data: %s, length: %d", sa.data, n);
            id = sendarticle_1(&sa, cl);
            if(id != NULL) {
                printf("%d\n", *id);
            }
            break;
        case 'f':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            ra = retrievearticle_1(&ar, cl);
            fwrite(ra->data, MAXLEN2, 1, stdout);
            //printf("%d", strlen(ra->data));
            //printf("%s",ra->data);
            break;
        case 'i':
            cvalue = optarg;
            break;
        case 'r':
            cvalue = optarg;
            break;
        case 'l':
            break;
        case '?':
            if (optopt == 'c')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();
        }
    return 0;
}
