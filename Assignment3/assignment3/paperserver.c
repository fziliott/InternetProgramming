#include "storage.h"

#define MAXLENGTH 1024

article_info* retrieveArticleInfo(int id) {
	article_info* ai=malloc(sizeof(article_info));
	ai->name=malloc(sizeof(filename));
	ai->author=malloc(sizeof(authorname));
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *token;
    int artID;
    int found = 0;
    file = fopen("articles/articles.txt", "rb");
    if (file == NULL) {
        return NULL;
    } else {
        while ((read = getline(&line, &len, file)) != -1 && !found) {
            printf("Retrieved line of length %zu :\n", read);
            printf("%s", line);
            if((token = strtok(line, "\t\n"))  == NULL) {
                return NULL;
            }
            artID = atoi(token);
            printf("%d,%d\n", artID, id);
            if(id == artID) {
                found = 1;
                if((token = strtok(NULL, "\t\n"))  == NULL) {
                    return NULL;
                }
                strcpy(ai->author, token);
                printf("%s", token);
                if((token = strtok(NULL, "\t\n"))  == NULL) {
                    return NULL;
                }
                strcpy(ai->name, token);
                printf("%s", token);
            }
        }
    }
    fclose(file);
    return ai;
}

retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    article_info* ai;

    printf("Article: %d\n", request->articleID);
    FILE *file;
    char fileName[MAXLENGTH];
    char *dir = "articles/";
    char data[MAXLEN2];
    ai = retrieveArticleInfo(request->articleID);

    if(ai != NULL) {
        printf("found");
        strcpy(fileName, dir);
        printf("filename: \n%s -> len %d\n", fileName, strlen(fileName));
        strcat(fileName, ai->name);
        printf("filename: \n%s -> len %d\n", fileName, strlen(fileName));
        file = fopen(fileName, "rb");
        if (file == NULL) {
            printf("NULL");
            return NULL;
        }
        printf("A");
        int n=fread(ra.data, 1, MAXLEN2, file);
        printf("n: %d\n",n);
        //fwrite(ra.data,MAXLEN2,1,stdout);
        //printf("data: %d", strlen(ra.data));
        //ra.data=malloc(sizeof(MAXLEN2));
        //ra.data=data;
        fclose(file);
        return(&ra);
    }
    return NULL;
}

