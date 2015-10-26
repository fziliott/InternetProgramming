#include <stdio.h>
#include "../../paper/paperserver.h"
#include "../../ccgi/ccgi.h"

char buffer[MAXLEN];

char* stringify(struct article_info* ai){
    sprintf(buffer, "{\n\t\t\"id\" : \"%d\"\n\t\t\"title\" : \"%s\"\n\t\t\"author\" : \"%s\"\n}\n",ai->id,ai->name,ai->author);
    FILE* f=fopen("~/aa", "w");
    fprintf(f, "%s\n", buffer);
    //printf("%s\n", buffer);
    fclose(f);
    return buffer;
}

int main(int argc, char const *argv[]) {
    CLIENT *cl;

    article_list *al;
    article_info ai;
    char list[MAXLEN];
    strcpy(list, "[\n\t");

    cl = clnt_create(argv[1], ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        printf("Error requesting file list!");
        return 1;
    }
    printf("<\n");
    al = listarticle_1(NULL, cl);
    while(al != NULL && al->item != NULL && al->item->id != -1) {
        strcat(list, stringify(al->item));
        //printf("%s\n",stringify(al->item) );
        //printf( "%d\t%s\t%s\n", al->item->id, al->item->author, al->item->name);
        al = al->next;
    }
    strcat(list, "]\n");

    FILE* file=fopen("hh", "w+");
    fwrite(list, 1, strlen(list), file);
    /** Print the CGI response header, required for all HTML output. **/
    /** Note the extra \n, to send the blank line.                   **/
    printf("Content-Type: text/plain\n\n");
    printf("%s", list);

    exit(0) ;
}