#include "storage.h"
#include  <fcntl.h>

#define MAXLENGTH 1024



article_info *retrieveArticleInfo(int id) {
    static article_info ai;
    bzero(ai.name, MAXLEN);
    bzero(ai.author, MAXLEN);
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
                strcpy(ai.author, token);
                printf("%s", token);
                if((token = strtok(NULL, "\t\n"))  == NULL) {
                    return NULL;
                }
                strcpy(ai.name, token);
                printf("%s", token);
            }
        }
    }
    fclose(file);
    return &ai;
}

int *sendarticle_1_svc(sent_article *sa, struct svc_req *clrts) {
    FILE *file;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    int artID;
    char filename[MAXLENGTH];
    int found = 0;
    char *token;
    char buffer[MAXLENGTH];


    file = fopen("articles/articles.txt", "rb");
    if (file == NULL) {
        return -1;
    } else {
        printf("autor to search:%s file:\n",sa->author, sa->name );
        while ((read = getline(&line, &len, file)) != -1 && !found) {
            if((token = strtok(line, "\t\n"))  == NULL) {
                return -1;
            }
            artID = atoi(token);
            if((token = strtok(NULL, "\t\n"))  == NULL) {
                return -1;
            }
            printf("%s\n", token);
            if(strcmp(token, sa->author) == 0) {

                if((token = strtok(NULL, "\t\n"))  == NULL) {
                    return -1;
                }
                if(strcmp(token, sa->name) == 0 ) {
                    found = 1;
                }
            }
        }
        fclose(file);
        bzero(filename, MAXLENGTH);
        strcpy(filename, "articles/");
        strcat(filename, sa->name);
        printf("\nname:%s\n", filename );
        FILE *newArticle = fopen(filename, "w+");
        printf("opened\n");
        // fseek(newArticle, 0, SEEK_END);
        // int lSize = ftell(newArticle);
        // printf("size: %d\n", lSize);
        // fseek(newArticle, 0, SEEK_SET);


        //get dimension of stream
        int c = 0;
        long nPos = 0;
        /*while(nPos <= MAXLEN2) {
            c = (int)sa->data[nPos];
            if(c == -1) {
                break;
            }
            nPos++;
        }*/
        //write stream until EOF
        fwrite(sa->data, 1, sa->size, newArticle);
        //printf("data: %s\n", sa->data);
        //fputs(sa->data, newArticle);
        //printf("found:%d\n", found);
        fclose(newArticle);
        /*what about if we receive 2 files with same name but different author??*/
        //update information
        if(!found) {
            printf("not found\n");
            artID++; //new id
            sprintf(buffer, "%d\t%s\t%s\n", artID, sa->author, sa->name);
            //printf("%s\n", buffer );
            //open to append at the end
            file = fopen("./articles/articles.txt", "a");
            int num=fwrite(buffer, 1, strlen(buffer), file);
            printf("num: %d\n",num );
            fclose(file);
        }
    }
    return &artID;
}

retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    bzero(ra.data, MAXLEN2);
    article_info *ai;

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
        int n = fread(ra.data, 1, MAXLEN2, file);
        printf("n: %d\n", n);
        //fwrite(ra.data,MAXLEN2,1,stdout);
        //printf("data: %d", strlen(ra.data));
        //ra.data=malloc(sizeof(MAXLEN2));
        //ra.data=data;
        fclose(file);
        return(&ra);
    }
    return NULL;
}

