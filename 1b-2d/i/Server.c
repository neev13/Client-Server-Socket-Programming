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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 4000
#define PORT 9090
#define DATA_BUFFER 5000
#define MAX_CONNECTIONS 10
#define SA struct sockaddr

unsigned long long factorial(int x){
    unsigned long long number = 1 ;
    while (x>0){
        number=number*x ;
        x-- ;
    }
    return number;  
}

void func(int connfd, FILE *fp, struct sockaddr_in servaddr,struct sockaddr_in length)
{
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
int main(){
    fd_set read_fd_set;
    int sockfd_1, sockfd, connfd, connfd2, size;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t socklen;
    char buf[DATA_BUFFER];
    int all_connections[MAX_CONNECTIONS];

    FILE *newfile;
    newfile = fopen ("data.txt", "w");
    
    sockfd_1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_1 == -1){
        printf("socket creation has failed... \n");
        exit(0);
    }
    else{
        printf("Socket has successfully created..\n");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd_1, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind has failed...\n");
        exit(0);
    }
    else
        printf("Socket has successfully binded..\n");
   
    if ((listen(sockfd_1, 5)) != 0) {
        printf("Listen has failed...\n");
        exit(0);
    }
    else{
        printf("Server is listening..\n");
        size = sizeof(clientaddr);
    }

    for (int i=0; i<MAX_CONNECTIONS; i++) {
         all_connections[i] = 0;
     }
     //all_connections[0] = sockfd_1;
    int start = clock();
    while (1!=13) {

        FD_ZERO(&read_fd_set);
     
        FD_SET(sockfd_1, &read_fd_set);  
        sockfd = sockfd_1;  
             
        for ( int i = 0 ; i < MAX_CONNECTIONS ; i++){  
            connfd = all_connections[i];  
                 
            if(connfd > 0)  
                FD_SET(connfd, &read_fd_set);  
                 
            if(connfd > sockfd)  
                sockfd = connfd;  
        }  
             
        if (FD_ISSET(sockfd_1, &read_fd_set)){  
            connfd = accept(sockfd_1, (SA*)&clientaddr, &size);
 
            if (connfd < 0) {
                printf("server accept has failed...\n");
                exit(0);
            }
        }
        func(connfd, newfile, servaddr, clientaddr);
        int end = clock();
            double time = (double)(end-start)/CLOCKS_PER_SEC;
            printf("%f\n",time);
    }

    fclose(newfile);
    shutdown(sockfd_1, SHUT_RDWR);
   return 0;
}