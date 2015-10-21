const MAXLEN = 2048;
const FILELEN = 64000;


struct article_request {
    int articleID;
    int start;
};

struct retrieved_article {
    opaque data<>;
    int size;
    int total_size;
};

struct article_info {
    int id;
    string name<>;
    string author<>;
};

struct article_list{
    struct article_info* item;
    struct article_list *next;    
};

struct sent_article {
    string name<>;
    string author<>;
    opaque data<>;
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