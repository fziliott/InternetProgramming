#include <stdio.h>
#include "../../paper/paperserver.h"
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"

char buffer[MAXLEN];

char* stringify(struct article_info* ai){
    sprintf(buffer, "{\"id\" : \"%d\" , \"title\" : \"%s\" , \"author\" : \"%s\"}",ai->id,ai->name,ai->author);
    return buffer;
}

int main(int argc, char const *argv[]) {
    CLIENT *cl;

    article_list *al;
    article_info ai;
    char list[MAXLEN];
    strcpy(list, "[");

    cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        printf("Error requesting file list!");
        return 1;
    }

    al = listarticle_1(NULL, cl);
    int i =0;
    while(al != NULL && al->item != NULL && al->item->id != -1) {
        if(i!=0){
            strcat(list, ",");
        }
        strcat(list, stringify(al->item));
        i++;
        
        al = al->next;
    }
    strcat(list, "]\n");

    /** Print the CGI response header, required for all HTML output. **/
    /** Note the extra \n, to send the blank line.                   **/
    printf("Content-Type: text/plain\n\n");
    printf("%s\n", list);

    exit(0) ;
}