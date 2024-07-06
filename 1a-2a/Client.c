#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 4000
#define PORT 9090
#define SA struct sockaddr

void func(int connfd){
    
    //int n;
    for (int m=1; m<=20; m++) {
        printf("%d\n",m);
        if(write(connfd, &m, sizeof(m))<0){
            printf("error: write failed");
        }
        
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr;
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket has failed...\n");
        exit(0);
    }
    else
        printf("Socket is successfully created..\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))
        != 0) {
        printf("socket bind has failed...\n");
        exit(0);
    }
    else
        printf("socket has successfully binded..\n");
	
    func(sockfd);
    return 0;
}




