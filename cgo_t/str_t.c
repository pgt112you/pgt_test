#include<str_t.h>


char *concatenate1(char *s1, char *s2) {
    char *res = (char *)malloc(strlen(s1)+strlen(s2)+1);
    char *res_origin = res;
    bzero(res, strlen(s1)+strlen(s2)+1);
    res = memcpy(res, s1, strlen(s1));
    memcpy(res+strlen(s1), s2, strlen(s2));
    return res_origin;
}


void concatenate2(char *s1, char *s2, char *res) {
    res = memcpy(res, s1, strlen(s1));
    res = memcpy(res+strlen(s1), s2, strlen(s2));
}
