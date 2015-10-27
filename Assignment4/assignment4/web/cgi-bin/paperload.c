#include "../../paper/paperserver.h"
#include <stdio.h>
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"


char buffer[MAXLEN];

char *stringify(article_info *ai) {
    sprintf(buffer, "{\n\t\t\"id\" : \"%d\"\n\t\t\"title\" : \"%s\"\n\t\t\"author\" : \"%s\"\n}\n", ai->id, ai->name, ai->author);
    printf("%s\n", buffer);
    return buffer;
}

int main(int argc, char const *argv[]) {
    CLIENT *cl;

    article_list *al;
    sent_article sa;
    int* id;
    char author[MAXLEN];
	char error[MAXLEN];

    CGI_varlist *varlist = NULL;
    const char *name;
    CGI_value  *value;
    int i;
    char *content;



    varlist = CGI_get_post(NULL, "/tmp/cgi-upload-XXXXXX");

    value = CGI_lookup_all(varlist, "author");
    if (value == 0 || value[0] == 0|| strcmp("",value[0])==0) {
           strcpy(error, "No author was found");
    	printf("Location: ../papererror.php?reason=%s\n\n", error);
    	return 0;
    } else {
    	sa.author=malloc(strlen(value[0]));
        strcpy(sa.author, value[0]);
        int i;
    }
    value = CGI_lookup_all(varlist, "id");
    if (value == 0 || value[0] == 0|| strcmp("",value[0])==0) {
        strcpy(error, "No name for the file");
    	printf("Location: ../papererror.php?reason=%s\n\n", error);
    	return 0;
    } else {
    	sa.name=malloc(strlen(value[0]));

        strcpy(sa.name, value[0]);

    }


    value = CGI_lookup_all(varlist, "file");
     
    if (value == 0 || value[0] == 0) {
    	strcpy(error, "No file was found");
    	printf("Location: ../papererror.php?reason=%s\n\n", error);
    	return 0;
    } 


    FILE *file = fopen(value[0], "rb");

int size=-1;

    fseek(file, 0, SEEK_END);
    size=ftell(file);
    fseek(file, 0, SEEK_SET);


    sa.data.data_len = size;
    sa.size=size;
    sa.data.data_val = malloc(size);

fread(sa.data.data_val, 1, size, file);
 
    cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
    	strcpy(error, "Can't connect to server");
        printf("Location: ../papererror.php?reason=%s\n\n", error);

        return 1;
    }
   
    id = sendarticle_1(&sa, cl);
    int newId=*id;
    if (newId == -1)
        printf("Location: ../papererror.php?reason=%s\n\n", "Can't store the file");
    else
        printf("Location: ../paperinfo.php?id=%d&new=%s\n\n", newId, "Upload successful");
    return 0;
}