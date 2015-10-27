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
    //CGI_varlist *input;
    article_list *al;
    sent_article sa;

    CGI_varlist *varlist=NULL;
    const char *name;
    CGI_value  *value;
    int i;
    char* content;
 
           printf("Content-Type: text/plain\n\n");

    /* output all values of all variables and cookies */
/*
    for (name = CGI_first_name(varlist); name != 0;
        name = CGI_next_name(varlist))
    {
        value = CGI_lookup_all(varlist, 0);


        for (i = 0; value[i] != 0; i++) {
            printf("%s [%d] = %s\r\n", name, i, value[i]);
        }
    }*/

     varlist = CGI_get_all("/tmp/cgi-upload-XXXXXX");
     if(varlist==NULL){
     	printf("NULL\n");
     }
     else{
     	printf("NO NULL\n");
     }
    value = CGI_lookup_all(varlist, "file");
    if (value == 0 || value[1] == 0) {
        fputs("No file was uploadedd\r\n", stdout);
    }
    else {
        printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
            value[1], value[0]);
    }

    FILE* f=fopen(value[1],"rw");
    fwrite(stdout,1,800,f);

    value = CGI_lookup_all(varlist, "author");
    if (value == 0 || value[1] == 0) {
        fputs("No file was uploadedd\r\n", stdout);
    }
    else {
        printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
            value[1], value[0]);
    }
    value = CGI_lookup_all(varlist, "id");
    if (value == 0 || value[1] == 0) {
        fputs("No file was uploadedd\r\n", stdout);
    }
    else {
        printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
            value[1], value[0]);
    }
/*if ((varlist = CGI_get_all(0)) != 0){
 for (name = CGI_first_name(varlist); name != 0;
        name = CGI_next_name(varlist))
    {
        value = CGI_lookup_all(varlist, 0);


        for (i = 0; value[i] != 0; i++) {
            printf("%s [%d] = %s\r\n", name, i, value[i]);
        }
    }
}
else{
	printf("peto");
}*/
	 /*varlist = CGI_get_all("tmp/cgi-upload-XXXXXX");
    value = CGI_lookup_all(varlist, "file");
    if (value == 0 || value[1] == 0) {
        fputs("No file was uploaded\r\n", stdout);
    }
    else {
        printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
            value[1], value[0]);
    }*/

/*
    char* input = getenv("CONTENT_TYPE");
    printf("%s\n",input);
    int content_length = atoi(getenv("CONTENT_LENGTH"));
    printf("content-length: %d\n",content_length);

    content = (char *) malloc(sizeof(char) * content_length); 

fread(content, content_length, 1, stdin);
fwrite(content,sizeof(char),content_length,stdout);*/
/*varlist= CGI_get_post(NULL, "cgi-upload-XXXXXX");
    value = CGI_lookup_all(varlist, "file");
    if (value == 0 || value[1] == 0) {
        printf("No file was uploaded -> %d %d\r\n", value);
    }
    else {
        printf("Your file \"%s\" was uploaded to my file \"%s\"\r\n",
            value[1], value[0]);
    }*/

   /* cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        printf("Error requesting file list!");
        return 1;
    }*/
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
           // sa.data.data_len = 6;
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