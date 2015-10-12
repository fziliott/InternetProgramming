#include "storage.h"
#include  <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define MAXLENGTH 1024



char *articlesFile = "articles/articles.txt";


/* retrieve article info*/
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
    file = fopen(articlesFile, "rb");
    if (file == NULL) {
        return NULL;
    } else {
        ai.id = id;
        /*search article on articles list, retrieve author and name*/
        while ((read = getline(&line, &len, file)) != -1 && !found) {
            if((token = strtok(line, "\t\n"))  == NULL) {
                return NULL;
            }
            artID = atoi(token);
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

/*list all stored articles*/
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

    file = fopen(articlesFile, "rb");
    if (file == NULL) {
        return NULL;
    }
    int first = 1;
    /*parse articles file and create a list of article_info elements*/
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

/*retrieve article info*/
article_info *retrievearticleinfo_1_svc(article_request *ar, struct svc_req *cl) {
    return getArticleInfo(ar->articleID);
}


/*remove article given an article id*/
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

    strcpy(fileName, "articles/");
    strcat(fileName, ai->author);
    strcat(fileName, "/");
    strcat(fileName, ai->name);

    int status = remove(fileName);

    if( status == 0 ) { //if the article was correctly removed
        FILE *tmp = fopen("articles/tmp.txt", "w+");
        if (tmp == NULL) {
            return NULL;
        }
        FILE *file = fopen(articlesFile, "rb");
        if (file == NULL) {
            return NULL;
        }

        /*search the article entry on articles list and discard it*/
        while ((read = getline(&line, &len, file)) != -1 ) {
            strcpy(buffer, line);

            if((token = strtok(line, "\t\n"))  == NULL) {
                return NULL;
            }
            if(ar->articleID != atoi(token)) {
                fwrite(buffer, 1, strlen(buffer), tmp);
            }
        }
        fclose(file);
        fclose(tmp);
        remove(articlesFile);
        rename("articles/tmp.txt", articlesFile);



    } else {
        printf("Unable to delete the file\n");
        perror("Error");
        res = -1;
    }
    return &res;
}


/*send a new article*/
int *sendarticle_1_svc(sent_article *sa, struct svc_req *clrts) {
    FILE *file;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    static int artID = 0;
    static int error = -1;
    char filename[MAXLENGTH];
    int found = 0;
    int empty = 1;
    char *token;
    char buffer[MAXLENGTH];
    struct stat st = {0};
    if (stat("articles", &st) == -1) {
        mkdir("articles", 0700);
    }

    file = fopen(articlesFile, "rb+");
    if(file == NULL) { //if file does not exist, create it
        file = fopen(articlesFile, "wb");
    }

    /*search if the tuple (author,file) already exists*/
    while ((read = getline(&line, &len, file)) != -1 && !found) {
        empty = 0;
        if((token = strtok(line, "\t\n"))  == NULL) {
            return &error;
        }
        artID = atoi(token);
        if((token = strtok(NULL, "\t\n"))  == NULL) {
            return &error;
        }
        if(strcmp(token, sa->author) == 0) {

            if((token = strtok(NULL, "\t\n"))  == NULL) {
                return &error;
            }
            if(strcmp(token, sa->name) == 0 ) {
                found = 1;
            }
        }
    }
    printf("bzero\n");
    fclose(file);
    bzero(filename, MAXLENGTH);
    strcpy(filename, "articles/");
    strcat(filename, sa->author);
    printf("creazione\n");

    /*create the author folded if it doesn't already exists*/
    struct stat st1 = {0};
    if (stat(filename, &st1) == -1) {
        mkdir(filename, 0700);
    }
    strcat(filename, "/");
    strcat(filename, sa->name);

    /*update an existing article or create a new article*/
    FILE *newArticle = fopen(filename, "w+");
    fwrite(sa->data, 1, sa->size, newArticle);
    fclose(newArticle);

    /*if the article doesn't exists, add it into the file containing the list of all articles*/
    if(!found) {
        if(empty == 1) {
            sprintf(buffer, "%d\t%s\t%s", artID, sa->author, sa->name);
        } else {
            artID++; //new id
            sprintf(buffer, "\n%d\t%s\t%s", artID, sa->author, sa->name);
        }
        file = fopen(articlesFile, "a");
        int num = fwrite(buffer, 1, strlen(buffer), file);
        fclose(file);
    }

    return &artID;
}

/*retrieve article from ID*/
retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    article_info *ai;

    FILE *file;
    char fileName[MAXLENGTH];
    char *dir = "articles/";
    char data[FILELEN];
    ai = getArticleInfo(request->articleID);

    if(ai != NULL) {    //if the article was find
        strcpy(fileName, dir);
        strcat(fileName, ai->author);
        strcat(fileName, "/");
        strcat(fileName, ai->name);
        file = fopen(fileName, "rb");
        if (file == NULL) {
            return NULL;
        }
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        ra.data = malloc(sizeof(char) * size);
        int n = fread(ra.data, 1, sizeof(char) * size, file);
        ra.size = n;
        fclose(file);
        return(&ra);
    }
    return NULL;
}

