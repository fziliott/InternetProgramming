#include "paperserver.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>


struct paperitem {
    int id;
    char author[MAXLEN];
    char name[MAXLEN];
    char *data;
    int dataSize;
    struct paperitem *next;
};

int papersId;

struct paperitem *papers = NULL;

/*finalization function, for a safe closure*/
void handler() {
    printf("called handler\n");
    struct paperitem* item=papers;
    while(item!=NULL){
        struct paperitem* tmp=item->next;
        free(item->data);
        free(item);
        item=tmp;
    }
    papers=NULL;
    exit(0);
}

void setSignals(){
    signal(SIGINT, handler);
    signal(SIGTERM, handler);
    signal(SIGQUIT, handler);
    signal(SIGHUP, handler);
}

void printItems(struct paperitem *list) {
    int i = 0;
    printf("ora printo\n");
    while(list != NULL) {
        printf("\nElemento %d:\n", i);
        printf("id: %d\n", list->id);
        printf("author: %s\n", list->author );
        printf("name: %s\n", list->name );
        printf("data size %d\n", list->dataSize );
        list = list->next;
        i++;
    }
}


/* retrieve article info*/
int getArticleInfo(int id, article_info *ai) {
    int artID;
    int found = 0;

    struct paperitem *item;
    item = papers;
    while(item != NULL) {
        artID = item->id;
        if(id == artID) {
            found = 1;
            ai->author = malloc(strlen(item->author));
            strcpy(ai->author, item->author);
            ai->name = malloc(strlen(item->name));
            strcpy(ai->name, item->name);
            return found;
        }
        item = item->next;
    }

    return found;
}

struct paperitem *getArticle(int id) {
    int artID;
    int found = 0;

    struct paperitem *item = papers;

    while(item != NULL && !found) {
        if(id == item->id) {
            return item;
        } else {
            item = item->next;
        }
    }
    return NULL;
}



/*list all stored articles*/
article_list *listarticle_1_svc(void *v, struct svc_req *cl) {
    int first = 1;

    static article_list articleList;
    article_list *elem = &articleList;

    struct paperitem *listItem = papers;
    while(listItem != NULL) {
        if(first) {
            elem = &articleList;
            first = 0;
        } else {
            elem->next = malloc(sizeof(article_list));
            elem = elem->next;
        }
        struct article_info *ai = malloc(sizeof(article_info));

        ai->id = listItem->id;
        ai->author = malloc(strlen(listItem->author));
        strcpy(ai->author, listItem->author);
        ai->name = malloc(strlen(listItem->name));
        strcpy(ai->name, listItem->name);

        elem->item = ai;

        elem->next = NULL;

        listItem = listItem->next;
    }

    if(articleList.item == NULL) {
        articleList.item = malloc(sizeof(article_info));
        articleList.item->id = -1;
    }
    return &articleList;
}

/*retrieve article info*/
article_info *retrievearticleinfo_1_svc(article_request *ar, struct svc_req *cl) {
    static article_info ai;
    int res = getArticleInfo(ar->articleID, &ai);
    if(res == 1) {
        printf("trovato\n");
        ai.id = ar->articleID;
    } else {
        printf("non trov\n");
        ai.id = -1;
    }
    return &ai;
}


/*remove article given an article id*/
int *removearticle_1_svc(article_request *ar, struct svc_req *cl) {
    static int res = -1;
    int artID = 0;
    int found = 0;

    struct paperitem *prev;
    struct paperitem *item;


    if(papers == NULL) {
        res = -1;
        return &res;
    } else if( papers->id == ar->articleID) {
        free(papers->data);
        item = papers;
        papers = papers->next;
        free(item);
        res = 0;
        return &res;
    } else {
        prev = papers;
        item = prev->next;
    }

    while(item != NULL && !found) {
        if(ar->articleID == item->id) {
            //found
            found = 1;
            prev->next = item->next;

            free(item->data);
            free(item);
            res = 0;
            return &res;
        } else {
            prev = item;
            item = item->next;
        }
    }

    return &res;
}


/*send a new article*/
int *sendarticle_1_svc(sent_article *sa, struct svc_req *clrts) {
    setSignals();

    struct paperitem *item;
    struct paperitem *prev;
    static int res;
    int artID = -1;
    int found = 0;

    prev = NULL;
        item = papers;
        while(item!=NULL) {
                artID = item->id;

            if(strcmp(item->author, sa->author) == 0 && strcmp(item->name, sa->name) == 0) {
                //found
                found = 1;
                //have to update
                free(item->data);
                item->data=malloc(sa->data.data_len);
                memcpy(item->data, sa->data.data_val, sa->data.data_len);
                item->dataSize = sa->data.data_len;
                res=artID;
                return &res;
            } 
                prev = item;
                item = item->next;
            
        }
        item=malloc(sizeof(struct paperitem));
        artID++;
        item->id = artID;

        strcpy(item->author, sa->author);
        strcpy(item->name, sa->name);
        item->data = malloc(sa->data.data_len);
        memcpy(item->data, sa->data.data_val, sa->data.data_len);
        item->dataSize = sa->data.data_len;
        item->next = NULL;

        if(papers==NULL){
            papers=item;
        }else{
            prev->next=item;
        }
    //printItems(papers);
res=artID;
    return &res;
}

/*retrieve article from ID*/
retrieved_article *retrievearticle_1_svc(article_request *request, struct svc_req *rqstp) {
    static retrieved_article ra;
    article_info *ai = malloc(sizeof(article_info));

    struct paperitem *item = getArticle(request->articleID);

    if(item == NULL) {
        ra.size = -1;
        return &ra;
    }

    ra.data.data_val = malloc(item->dataSize);
    memcpy(ra.data.data_val, item->data, item->dataSize);

    ra.data.data_len = item->dataSize;
    printf("%d\n", ra.size );
    return &ra;
}

