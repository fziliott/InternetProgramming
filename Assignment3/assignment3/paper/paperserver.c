#include "paperserver.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char *articlesFile = "articles/articles.txt";
int primo=0;
FILE *openArticles() {
    FILE *file;
    struct stat st = {0};
    if (stat("articles", &st) == -1) {
        mkdir("articles", 0700);
    }

    file = fopen(articlesFile, "rb+");
    if (file == NULL) {
        return NULL;
    } else {
        return file;
    }
}
//void createDataList(struct sent_article *sa, int size, FILE *file);

/* retrieve article info*/
int getArticleInfo(int id, article_info *ai) {
    bzero(ai->name, MAXLEN);
    //bzero(ai->author, MAXLEN);
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *token = NULL;
    int artID;
    int found = 0;

    file = openArticles();

    if(file == NULL) {
        return -1;
    }
    ai->id = id;
    /*search article on articles list, retrieve author and name*/
    while ((read = getline(&line, &len, file)) != -1 && !found) {
        if((token = strtok(line, "\t\n"))  == NULL) {
            return -1;
        }
        artID = atoi(token);
        if(id == artID) {
            found = 1;
            if((token = strtok(NULL, "\t\n"))  == NULL) {
                return -1;
            }
            //ai->author=strdup(token);
            strcpy(ai->author, token);
            if((token = strtok(NULL, "\t\n"))  == NULL) {
                return -1;
            }
            //ai->name=strdup(token);
            strcpy(ai->name, token);
        }
    }
    if(read == -1)
        return -1;

    fclose(file);
    return found;
}

/*list all stored articles*/
article_list *listarticle_1_svc(void *v, struct svc_req *cl) {
    static article_list articleList;
    article_list *head = NULL;
    FILE *file;
    size_t read;
    char *line = NULL;
    size_t len = 0;
    char *token;
    article_list *elem = &articleList;

    //bzero(&articleList, sizeof(article_list));

    file = openArticles();

    if (file == NULL) {
        articleList.item = NULL;

        printf("return null\n");
        return &articleList;
    }

    int first = 1;
    /*parse articles file and create a list of article_info elements*/
    while ((read = getline(&line, &len, file)) != -1 ) {
        if(first) {
            elem = &articleList;
            first = 0;
        } else {
            elem->next = malloc(sizeof(article_list));
            elem = elem->next;
        }
        struct article_info *ai = malloc(sizeof(article_info));
        if((token = strtok(line, "\t\n"))  == NULL) {
            if(articleList.item != NULL)
                free(articleList.item);
            articleList.item = NULL;
            return &articleList;
        }
        ai->id = atoi(token);
        if((token = strtok(NULL, "\t\n"))  == NULL) {
            if(articleList.item != NULL)
                free(articleList.item);
            articleList.item = NULL;
            return &articleList;
        }
            //ai->author=strdup(token);

        strcpy(ai->author, token);
        if((token = strtok(NULL, "\t\n"))  == NULL) {
            if(articleList.item != NULL)
                free(articleList.item);
            articleList.item = NULL;
            return &articleList;
        }
        //ai->name=strdup(token);
        strcpy(ai->name, token);
        elem->item = ai;
        elem->next = NULL;
    }
    fclose(file);
    return &articleList;
}

/*retrieve article info*/
article_info *retrievearticleinfo_1_svc(article_request *ar, struct svc_req *cl) {
    static article_info ai;
    int res = getArticleInfo(ar->articleID, &ai);
    if(res != -1)
        printf("%s, %s\n", ai.author, ai.name );
    return &ai;
}


/*remove article given an article id*/
int *removearticle_1_svc(article_request *ar, struct svc_req *cl) {
    static int res = 0;
    char *line = NULL;
    char buffer[MAXLEN];
    ssize_t len = 0;
    size_t read;
    char *token = NULL;
    article_info *ai = malloc(sizeof(article_info));
    getArticleInfo(ar->articleID, ai);
    if(ai == NULL) {
        res = -1;
        return (&res);
    }

    char fileName[MAXLEN];
    bzero(fileName, MAXLEN);

    strcpy(fileName, "articles/");
    strcat(fileName, ai->author);
    strcat(fileName, "/");
    strcat(fileName, ai->name);

    int status = remove(fileName);

    if( status == 0 ) { //if the article was correctly removed
        FILE *tmp = fopen("articles/tmp.txt", "w+");
        if (tmp == NULL) {
            res = -1;
            return &res;
        }
        FILE *file = fopen(articlesFile, "rb+");
        if (file == NULL) {
            res = -1;
            return &res;
        }

        /*search the article entry on articles list and discard it*/
        while ((read = getline(&line, &len, file)) != -1 ) {
            strcpy(buffer, line);

            if((token = strtok(line, "\t\n"))  == NULL) {
                res = -1;
                return &res;
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
    FILE *newArticle;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    static int artID = 0;
    static int error = -1;
    char filename[MAXLEN];
    int found = 0;
    int empty = 1;
    char *token;
    char buffer[MAXLEN];

    bzero(filename, MAXLEN);
    strcpy(filename, "articles/");
    struct stat st = {0};
    if (stat(filename, &st) == -1) {
        mkdir(filename, 0700);
    }
    strcat(filename, sa->name);
    /*create the author folded if it doesn't already exists*/
    /*struct stat st1 = {0};
    if (stat(filename, &st1) == -1) {
        mkdir(filename, 0700);
    }
    strcat(filename, "/");
    strcat(filename, sa->name);*/
    file = openArticles();
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

    if(sa->start == 0 || found == 1) {
        /*update an existing article or create a new article*/
        newArticle = fopen(filename, "w+");
    } else {
        /*append to an existing article */
        newArticle = fopen(filename, "a");
    }
    if(sa->size > 0) {
        fwrite(sa->data, 1, sa->size, newArticle );
    }
    fclose(newArticle);

    if(sa->finish == 1) {
        /*if the article doesn't exists, add it into the file containing the list of all articles*/
        if(!found) {
            if(empty == 1) {
                sprintf(buffer, "%d\t%s\t%s", artID, sa->author, sa->name);
            } else {
                artID++; //new id
                sprintf(buffer, "\n%d\t%s\t%s", artID, sa->author, sa->name);
            }
            file = fopen(articlesFile, "a");
            fwrite(buffer, 1, strlen(buffer), file);
            fclose(file);
        }
    }

    return &artID;
}

/*retrieve article from ID*/
retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    article_info *ai = malloc(sizeof(article_info));

    FILE *file;
    char fileName[MAXLEN];
    char *dir = "articles/";
    getArticleInfo(request->articleID, ai);

    if(ai != NULL) {    //if the article was find
        strcpy(fileName, dir);
        //strcat(fileName, ai->author);
        //strcat(fileName, "/");
        strcat(fileName, ai->name);
        file = fopen(fileName, "rb");
        if (file == NULL) {
            ra.total_size = -1;
            return &ra;
        }
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        ra.total_size = size;

        //ra.data = malloc(sizeof(char) * size);
        //bzero(ra.data, sizeof(article));
        fseek(file, request->start, SEEK_SET);

        int n = fread(ra.data, 1, sizeof(char) * sizeof(article), file);
        ra.size = n;
        //ra.size = n;
        fclose(file);
        return (&ra);
    }
    return NULL;
}

