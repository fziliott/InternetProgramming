#include "../../paper/paperserver.h"
#include <stdio.h>
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"

char buffer[MAXLEN];

char* stringify(article_info* ai){
    sprintf(buffer, "{\n\t\t\"id\" : \"%d\"\n\t\t\"title\" : \"%s\"\n\t\t\"author\" : \"%s\"\n}\n",ai->id,ai->name,ai->author);
    printf("%s\n", buffer);
    return buffer;
}

void main(int argc, char const *argv[]) {
    CLIENT *cl;
    CGI_varlist *input;
    article_list *al;
    sent_article sa;
    char* name;
 
           printf("Content-Type: text/plain\n\n");

    cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        printf("Error requesting file list!");
        return 1;
    }
    //input = CGI_get_post(NULL, NULL);
    //printf(" input %s\n",input );
/*
    while(name=CGI_next_name(input) != NULL){
        if(strcmp(name, "author")==0 ){
        strcpy(sa.author, CGI_lookup(name, input));
        }
        if(strcmp(name, "id")==0 ){
        strcpy(sa.name, CGI_lookup(name, input));

        }*/
//          we need the size of the file
        /*if(strcmp(name, "size")==0 ){
            sa.data.data_len= atoi(CGI_lookup(name, input));
        }*/
            sa.data.data_len = 6;
        /*if(strcmp(name, "file")==0 ){
            sa.data.data_val = malloc(sa.data.data_len);
            memcpy(sa.data.data_val, "hello", sa.data.data_len);

            //memcpy(sa.data.data_val, CGI_lookup(name, input), sa.data.data_len);
        }*/

    
   // int id=sendarticle_1(&sa, cl);
    if (id==-1)
        printf("Location: papererror.php?reason=%d\n\n",id);
    else 
        printf("Location: paperinfo.php?id=%d\n\n",id);
    exit(0) ;
}