#include "storage.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    CLIENT *cl;
    article_request ar;
    retrieved_article *ra;
    char *cvalue = NULL;
    int c;

    cl = clnt_create(argv[1], ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        perror("Error creating RPC client!");
        return 1;
    }

    while ((c = getopt (argc, argv, "ha:::f:i:r:l")) != -1)
        switch (c) {
        case 'h':
            break;
        case 'a':
            cvalue = optarg;
            break;
        case 'f':
            cvalue = optarg;
            ar.articleID = atoi(cvalue);
            ra = retrievearticle_1(&ar, cl);
            fwrite(ra->data,MAXLEN2,1,stdout);
            printf("%d", strlen(ra->data));
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
