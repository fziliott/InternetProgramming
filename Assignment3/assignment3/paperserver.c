#include "storage.h"
#include  <fcntl.h>

#define MAXLENGTH 1024



article_info *getArticleInfo(int id) {
    static article_info ai;
    bzero(ai.name, MAXLEN);
    bzero(ai.author, MAXLEN);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *token = NULL;
    int artID;
    int found = 0;
    file = fopen("articles/articles.txt", "rb");
    if (file == NULL) {
        return NULL;
    } else {
        ai.id = id;
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
                if((token = strtok(NULL, "\t\n"))  == NULL) {
                    return NULL;
                }
                strcpy(ai.name, token);
            }
        }
    }
    fclose(file);
    return &ai;
}

article_list *listarticle_1_svc(void *v, struct svc_req *cl) {
    static article_list *articleList;
    article_list *head = NULL;
    FILE *file;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    char *token;
    article_list *elem = &articleList;

    bzero(&articleList, sizeof(article_list));

    file = fopen("articles/articles.txt", "rb");
    if (file == NULL) {
        return NULL;
    }
    int first = 1;
    while ((read = getline(&line, &len, file)) != -1 ) {
        if(first) {
            articleList = malloc(sizeof(article_list));
            elem = articleList;
            first = 0;
        } else {
            elem->next = malloc(sizeof(article_list));
            elem = elem->next;
        }
        struct article_info *ai = malloc(sizeof(article_info));
        if((token = strtok(line, "\t\n"))  == NULL) {
            return NULL;
        }
        ai->id = atoi(token);
        if((token = strtok(NULL, "\t\n"))  == NULL) {
            return NULL;
        }
        strcpy(ai->author, token);
        printf("author:%s", token);
        if((token = strtok(NULL, "\t\n"))  == NULL) {
            return NULL;
        }
        strcpy(ai->name, token);
        elem->item = ai;
        elem->next = NULL;
    }

    fclose(file);
    return articleList;
}

article_info *retrievearticleinfo_1_svc(article_request *ar, struct svc_req *cl) {
    return getArticleInfo(ar->articleID);
}

int *removearticle_1_svc(article_request *ar, struct svc_req *cl) {
    static int res = 0;
    char *line = NULL;
    char buffer[MAXLENGTH];
    ssize_t len = 0;
    size_t read;
    char *token = NULL;
    article_info *ai = getArticleInfo(ar->articleID);
    if(ai == NULL)
        return -1;

    char fileName[MAXLENGTH];
    bzero(fileName, MAXLENGTH);

    strcpy(fileName, "./articles/");
    strcat(fileName, ai->name);

    int status = remove(fileName);

    if( status == 0 ) {
        FILE *tmp = fopen("articles/tmp.txt", "w+");
        if (tmp == NULL) {
            return NULL;
        }
        FILE *file = fopen("articles/articles.txt", "rb");
        if (file == NULL) {
            return NULL;
        }

        while ((read = getline(&line, &len, file)) != -1 ) {
            strcpy(buffer, line);
            printf("%s\n", buffer);

            if((token = strtok(line, "\t\n"))  == NULL) {
                return NULL;
            }
            if(ar->articleID != atoi(token)) {
                printf("writing\n");
                //fputs(buffer, tmp);
                fwrite(buffer, 1, strlen(buffer), tmp);
            }
        }
        printf("renaming\n");
        fclose(file);
        fclose(tmp);
        remove("articles/articles.txt");
        rename("articles/tmp.txt", "articles/articles.txt");

    } else {
        printf("Unable to delete the file\n");
        perror("Error");
        res = -1;
    }
    printf("res:%d\n",  res);
    return &res;
}

int *sendarticle_1_svc(sent_article *sa, struct svc_req *clrts) {
    FILE *file;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    int artID = 0;
    char filename[MAXLENGTH];
    int found = 0;
    int empty = 1;
    char *token;
    char buffer[MAXLENGTH];


    file = fopen("articles/articles.txt", "rb");
    if (file == NULL) {
        return -1;
    } else {
        printf("autor to search:%s file:\n", sa->author, sa->name );
        while ((read = getline(&line, &len, file)) != -1 && !found) {
            empty = 0;
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

            if(empty == 1) {
                sprintf(buffer, "%d\t%s\t%s", artID, sa->author, sa->name);
            } else {
                artID++; //new id
                sprintf(buffer, "\n%d\t%s\t%s", artID, sa->author, sa->name);
            }
            file = fopen("./articles/articles.txt", "a");
            int num = fwrite(buffer, 1, strlen(buffer), file);
            printf("num: %d\n", num );
            fclose(file);
        }
    }
    return &artID;
}

retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    //bzero(ra.data, FILELEN);
    article_info *ai;

    printf("Article: %d\n", request->articleID);
    FILE *file;
    char fileName[MAXLENGTH];
    char *dir = "articles/";
    char data[FILELEN];
    ai = getArticleInfo(request->articleID);

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
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        ra.data=malloc(sizeof(char)*size);
        int n = fread(ra.data, 1, sizeof(char)*size, file);
        ra.size = n;
        printf("n: %d\n", n);
        //fwrite(ra.data,FILELEN,1,stdout);
        //printf("data: %d", strlen(ra.data));
        //ra.data=malloc(sizeof(FILELEN));
        //ra.data=data;
        fclose(file);
        return(&ra);
    }
    return NULL;
}

