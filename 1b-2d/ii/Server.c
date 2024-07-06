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
#include <string.h>
#include <errno.h>
#include <sys/poll.h>
#include <time.h>
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
    int sockfd_1, sockfd, connfd, size;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t socklen;
    char buf[DATA_BUFFER];
    int all_connections[MAX_CONNECTIONS];
    struct pollfd *pfds;
    int nfds = MAX_CONNECTIONS-1;
    struct pollfd pollfd_1[MAX_CONNECTIONS + 1];



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
    pollfd_1[0].fd = sockfd_1;
    pollfd_1[0].events = POLLIN;
    int client_user = 0;
    for (int i=0; i<MAX_CONNECTIONS; i++) {
        pollfd_1[0].fd = sockfd_1;
        pollfd_1[0].events = POLLIN;
     }
    int start = clock();
    while (1!=13) {
        connfd = poll(pollfd_1, client_user+1, 5000);
        if(connfd>=0){
            if (pollfd_1[0].revents & POLLIN) {
                /* accept the new connection */
                printf("Returned fd is %d (server's fd)\n", sockfd_1);
                int new_fd = accept(sockfd_1, (SA*)&clientaddr, &size);
                
                func(new_fd,newfile, servaddr, clientaddr);
                if (new_fd >= 0) {
                    printf ("Accepted a new connection with fd: %d\n", new_fd); 
                    for (int i=1; i < MAX_CONNECTIONS; i++){
                        if (pollfd_1[i].fd == 0) {
                            pollfd_1[i].fd = new_fd;
                            pollfd_1[i].events = POLLIN;
                            client_user++;
                            break;;
                        }
                    }
                }
                else{
                    fprintf(stderr, "accept failed\n");
                }
                connfd--;
                if(!connfd) continue;
            }
             /* Check if the fd with event is a non-server fd */
            for (int i=1; i<MAX_CONNECTIONS+1; i++) {
                 if (pollfd_1[i].fd>0 && pollfd_1[i].revents & POLLIN) {
                    memset(buf, 0, sizeof(buf));
                    int bufSize = read(pollfd_1[i].fd, buf, DATA_BUFFER - 1);
                    if(bufSize == -1 || bufSize == 0){
                        pollfd_1[i].fd = 0;
                        pollfd_1[i].events = 0;
                        pollfd_1[i].revents = 0;
                        client_user--;
                    }
                    else{
                        printf("From client: %s\n", buf);
                        // func(connfd, newfile, servaddr, clientaddr);
                    }
                 }
            }
        }
        else{
            printf("failed to connect.\n");
        }
        int end = clock();
            double time = (double)(end-start)/CLOCKS_PER_SEC;
            printf("%f\n",time);
    }

    fclose(newfile);
    shutdown(sockfd_1, SHUT_RDWR);
    return 0;
}