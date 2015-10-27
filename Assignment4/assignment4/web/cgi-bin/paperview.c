#include <stdio.h>
#include "../../paper/paperserver.h"
#include "../../ccgi/ccgi.h"
#include "../../paper/config.h"
#include <string.h>

#define NUM_TYPES 14


char *mimeTypes[] = {"txt", "text/plain", "pdf", "application/pdf", "doc", "application/msword", "docm", "application/vndms-worddocumentmacroEnabled12", "docx", "application/vndopenxmlformats-officedocumentwordprocessingmldocument", "odt", "application/vndoasisopendocumenttext", "html", "text/html", "rtx", "text/richtext", "zip", "application/zip", "tar", "application/x-tar", "gz", "application/x-gzip", "rtf", "application/rtf", "ppt", "application/powerpoint", "xls", "application/vnd.ms-excel"};

int main(int argc, char const *argv[]) {
    CLIENT *cl;
    char *input;
    int id;
    retrieved_article *ra;
    article_info *ai;
    static article_request ar;
    char mime[MAXLEN];
    char error[MAXLEN];

    CGI_varlist *varlist;
    const char *name;
    CGI_value  *value;

    varlist = CGI_get_query(NULL);
    if(varlist == 0) {
        strcpy(error, "Can't get input parameters");
        printf("Location: ../papererror.php?reason=%s\n\n", error);

        return 0;
    }
    value = CGI_lookup_all(varlist, "id");
    if (value == 0 || value[0] == 0) {
        strcpy(error, "No id was found");
        printf("Location: ../papererror.php?reason=%s\n\n", error);
        return 0;
    } else {
        id = atoi(value[0]);
    }


    cl = clnt_create(PAPER_ADDRESS, ARTICLE_PROG, ARTICLE_VER, "tcp");
    if (cl == NULL) {
        strcpy(error, "Can't connect to remote server");
        printf("Location: ../papererror.php?reason=%s\n\n", error);
        return 0;
    }
    ar.articleID = id;
    ai = retrievearticleinfo_1(&ar, cl);
    ra = retrievearticle_1(&ar, cl);

    if(ai->id == -1){
    	strcpy(error, "Can't find the paper!");
        printf("Location: ../papererror.php?reason=%s\n\n", error);
        return 0;
    }

    char *ext = strrchr(ai->name, '.');
    if (!ext) {
        strcpy(mime, "application/octect-stream");
    } else {
        int i = 0;
        for(i = 0; i < NUM_TYPES; i = i + 2) {
            if(strcmp(mimeTypes[i], ext + 1) == 0) {
                strcpy(mime, mimeTypes[i + 1]);
                break;
            }
        }
        if(i > NUM_TYPES) {
            strcpy(mime, "application/octect-stream");
        }
    }


    printf("Content-Type: %s\n", mime);
    printf("Content-Disposition: inline; filename=\"%s\"\n\n", ai->name);

    fwrite(ra->data.data_val, sizeof(char), ra->data.data_len, stdout);


    return 0 ;
}