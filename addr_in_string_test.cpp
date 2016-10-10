#include <stdlib.h>
#include <stdio.h>

#include <strings.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>




int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    bzero(&cliaddr, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "180.149.136.95", &servaddr.sin_addr);
    servaddr.sin_port = htons(9112);
    std::string str((char *)&servaddr, sizeof(struct sockaddr_in));
    printf("=====%s size is %d\n", str.c_str(), str.size());
    cliaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.88.15.46", &cliaddr.sin_addr);
    cliaddr.sin_port = htons(9222);
    std::string str1((char *)&servaddr, sizeof(struct sockaddr_in));
    printf("=====%s size is %d\n", str1.c_str(), str1.size());
    //if (str1.compare(str) == 0) {
    if (memcmp(&str, &str1, sizeof(str)) == 0) {
        printf("str1 == str\n");
    }
    else {
        printf("str1 != str\n");
    }
    if (memcmp(&servaddr, &cliaddr, sizeof(struct sockaddr_in)) == 0) {
        printf("serv == cli\n");
    }
    else {
        printf("serv != cli\n");
    }
    return 0;
}
