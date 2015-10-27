#include "../../paper/paperserver.h"
#include <stdio.h>
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"

char buffer[MAXLEN];

char* stringify(struct article_info* ai){
    sprintf(buffer, "{\"id\" : \"%d\" , \"title\" : \"%s\" , \"author\" : \"%s\"}",ai->id,ai->name,ai->author);
    return buffer;
}

int main(int argc, char const *argv[]) {
    CLIENT *cl;
    char *input;
    int id;
    article_info *ai;
    static article_request ar;
    char result[MAXLEN];

     CGI_varlist *varlist;
    const char *name;
    CGI_value  *value;

    char error[MAXLEN];

    input = getenv("QUERY_STRING");

    if(input != NULL){

        varlist = CGI_get_query(NULL);
    if(varlist == 0) {
        printf("Content-Type: text/plain\n\n");
            printf("{}");

        return 0;
    }
    value = CGI_lookup_all(varlist, "id");
    if (value == 0 || value[0] == 0) {
        printf("Content-Type: text/plain\n\n");
            printf("{}");
        return 0;
    } else {
        id = atoi(value[0]);


            cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
            if (cl == NULL) {
                printf("Error requesting file list!");
                return 1;
            }
            ar.articleID = id;
            ai = retrievearticleinfo_1(&ar, cl);

            strcpy(result, stringify(ai));
            printf("Content-Type: text/plain\n\n");
            printf("%s", result);
        }
    }
    exit(0) ;
}