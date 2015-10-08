const MAXLEN = 1024;
const MAXLEN2 = 1000000;

typedef char filename[MAXLEN];
typedef char authorname[MAXLEN];
typedef char articleslist[MAXLEN];
typedef opaque article[MAXLEN2];

struct article_request {
    int articleID;
};



struct retrieved_article {
    article data;
};

struct article_info {
    filename name;
    authorname author;
};


struct sent_article {
    filename name;
    authorname author;
    article data;
};

struct article_list {
	articleslist articles; 
};



program ARTICLE_PROG {
    version ARTICLE_VER {
        retrieved_article retrieveArticle(article_request) = 1;
        int sendArticle(sent_article) = 2;
        /*article_info retrieveArticleInfo(article_request) = 3;
        int removeArticle(article_request) = 4;
        article_list listArticle() = 5;*/
    } = 1;
} = 0x20000011;