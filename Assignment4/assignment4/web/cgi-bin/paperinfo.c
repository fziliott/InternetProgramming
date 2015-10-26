#include "../../paper/paperserver.h"
#include <stdio.h>
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"

char buffer[MAXLEN];

char *stringify(article_info *ai) {
    sprintf(buffer, "{\n\t\t\"id\" : \"%d\"\n\t\t\"title\" : \"%s\"\n\t\t\"author\" : \"%s\"\n}\n", ai->id, ai->name, ai->author);
    return buffer;
}

void main(int argc, char const *argv[]) {
    CLIENT *cl;
    char *input;
    int id;
    article_info *ai;
    static article_request ar;
    char result[MAXLEN];

    input = getenv("QUERY_STRING");

    if(input != NULL){

        if(scanf(input, "id=%d", &id) != 1) {
            printf("<p>Error, invalid input</p>\n");
        } else {
            cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
            if (cl == NULL) {
                printf("Error requesting file list!");
                return 1;
            }
            ar.articleID = id;
            ai = retrievearticleinfo_1(&ar, cl);

            strcpy(result, stringify(ai));
            printf("Content-Type: application/json\n\n");
            printf("%s", result);
        }
    }
    exit(0) ;
}