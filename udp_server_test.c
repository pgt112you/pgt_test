#include <stdlib.h>
#include <stdio.h>

#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>




int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9112);
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    char buf[1024];
    bzero(buf, 1024);
    struct sockaddr src_addr;
    socklen_t src_addr_len;
    for (;;) {
        int len = sizeof(servaddr);
        int n = recvfrom(sockfd, buf, 1024, 0, &src_addr, &src_addr_len);
        char *sendbuf = "hello world";
        sendto(sockfd, sendbuf, strlen(sendbuf)+1, 0, &src_addr, src_addr_len);
        printf("receive message\n");
    }
}
