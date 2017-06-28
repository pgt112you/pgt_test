#include<stdlib.h>
#include<stdio.h>
#include <string.h>


char *concatenate1(char *s1, char *s2);
void concatenate2(char *s1, char *s2, char *res);

//char *concatenate1(char *s1, char *s2) {
//    char *res = (char *)malloc(strlen(s1)+strlen(s2)+1);
//    bzero(res, strlen(s1)+strlen(s2)+1);
//    res = memcpy(res, s1, strlen(s1));
//    memcpy(res+strlen(s1), s2, strlen(s2));
//    return res;
//}
//
//
//void concatenate2(char *s1, char *s2, char *res) {
//    res = memcpy(res, s1, strlen(s1));
//    res = memcpy(res+strlen(s1), s2, strlen(s2));
//}
