#include <stdio.h>
#include "../../paper/paperserver.h"
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"

char buffer[MAXLEN];

char* stringify(struct article_info* ai){
    sprintf(buffer, "{\n\t\t\"id\" : \"%d\"\n\t\t\"title\" : \"%s\"\n\t\t\"author\" : \"%s\"\n}\n",ai->id,ai->name,ai->author);
    return buffer;
}

int main(int argc, char const *argv[]) {
    CLIENT *cl;

    article_list *al;
    article_info ai;
    char list[MAXLEN];
    strcpy(list, "[\n\t");

    cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        printf("Error requesting file list!");
        return 1;
    }

    al = listarticle_1(NULL, cl);
    
    while(al != NULL && al->item != NULL && al->item->id != -1) {
        strcat(list, stringify(al->item));
        //printf("%s\n",stringify(al->item) );
        //printf( "%d\t%s\t%s\n", al->item->id, al->item->author, al->item->name);
        al = al->next;
    }
    strcat(list, "]\n");

    /** Print the CGI response header, required for all HTML output. **/
    /** Note the extra \n, to send the blank line.                   **/
    printf("Content-Type: text/plain\n\n");
    printf("%s\n", list);

    exit(0) ;
}