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

long long factorial(int x){
    long long number = 1 ;
    while (x>0){
        number=number*x ;
        x-- ;
    }
    return number;  
}

void * func(int connfd, FILE *fp, struct sockaddr_in servaddr, struct sockaddr_in clientaddr)
{

    // int m;
    // for (int n=1; n<=20; n++) {
    //     int a = read(connfd, &m, sizeof(m));
        
    //     if(a<0){
    //         printf("value is not right");
    //         exit(0);
    //     }
    //     if(a==0){
    //         break;
    //     }
    //     long long final  = factorial(m);
    //     printf("Factorial for %d: %lld\n",n ,final);
    //     char snum[100];
    //     sprintf(snum,"%lld",final);
    //     fprintf(fp, "Request Number: %d , Factorial: %lld , IP Adress: %s , Port Adress: %d \n", m, final, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    //     send(connfd, snum, strlen(snum), 0);
    // } 
    // fflush(fp);
    // close(connfd);
    char buffer[1024]={0};
    //bzero(buffer,1024);
    //int a = read(connfd, &buffer, 1024);
        
    // if(a<0){
    //     printf("value is not right");
    //     exit(0);
    // }
        // if(a==0){
        //     close(connfd);
        //     break;
        // }
    //int m = atoi(buffer);
    //bzero(buffer,1024);
    //unsigned long long final  = factorial(m);
    for(int i=1; i<=20; i++){
        int a = read(connfd, &buffer, 1024);
        int m = atoi(buffer);
        printf("Request received from client: %d \n", m);
        long long f = factorial(m);
        char snum[100];
        sprintf(snum,"%lld",f);
        fprintf(fp, "Request Number: %d , Factorial: %lld , IP Adress: %s , Port Adress: %d \n", m, f, inet_ntoa(length.sin_addr), ntohs(length.sin_port));
            
        send(connfd, snum, strlen(snum), 0);
    }
    //printf("Factorial of request %s sent\n", buffer);
        
    fflush(fp);
    //close(connfd);
}
int main()
{
    int sockfd, connfd, size;
    struct sockaddr_in servaddr, length;

    FILE *newfile;
    newfile = fopen ("data.txt", "w");
   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        printf("socket creation has failed... \n");
        exit(0);
    }
    else{
        printf("Socket has successfully created..\n");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind has failed...\n");
        exit(0);
    }
    else
        printf("Socket has successfully binded..\n");
   
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen has failed...\n");
        exit(0);
    }
    else
        printf("Server is listening..\n");
    size = sizeof(length);

    connfd = accept(sockfd, (SA*)&length, &size);
    if (connfd < 0) {
        printf("server accept has failed...\n");
        exit(0);
    }
    func(connfd, newfile, servaddr, length);
    fclose(newfile);
}