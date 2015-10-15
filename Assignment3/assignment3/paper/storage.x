const MAXLEN = 1024;
const FILELEN = 1000000;

typedef char filename[PATH_MAX];
typedef char authorname[MAXLEN];
typedef char articleslist[MAXLEN];
typedef char article[FILELEN];

struct article_request {
    int articleID;
    int start;
};

struct retrieved_article {
    article data;
    int size;
    int total_size;
};

struct article_info {
    int id;
    filename name;
    authorname author;
};

struct article_list{
    struct article_info* item;
    struct article_list *next;    
};

struct sent_article {
    filename name;
    authorname author;
    article data;
    int start;
    int size;
    int finish;
};

program ARTICLE_PROG {
    version ARTICLE_VER {
        retrieved_article retrieveArticle(article_request) = 1;
        int sendArticle(sent_article) = 2;
        article_info retrieveArticleInfo(article_request) = 3;
        int removeArticle(article_request) = 4;
        article_list listArticle() = 5;
    } = 1;
} = 0x20000011;