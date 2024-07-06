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
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#define MAX 4000
#define PORT 9090
#define SA struct sockaddr

sem_t x, y;
pthread_t tid;
pthread_t writerthreads[100];
//pthread_t readerthreads[100];
int readercount = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

unsigned long long factorial(int x){
    unsigned long long number = 1 ;
    while (x>0){
        number=number*x ;
        x-- ;
    }
    return number;  
}
void* writer(void* param)
{
    // Lock the semaphore
    sem_wait(&y);
    // Unlock the semaphore
    sem_post(&y);
    pthread_exit(NULL);
}

void func(int connfd, FILE *fp, struct sockaddr_in servaddr,struct sockaddr_in length,int start)
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
        if(m==20){
            int end = clock();
            double time = (double)(end-start)/CLOCKS_PER_SEC;
            printf("%f\n",time);
        }
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
    int sockfd, connfd, clientfd;
    struct sockaddr_in servaddr;
    struct sockaddr_in serverStorage;
    socklen_t addr_size;
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);

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
   
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind has failed...\n");
        exit(0);
    }
    else
        printf("Socket has successfully binded..\n");
   
    if ((listen(sockfd, 10)) != 0) {
        printf("Listen has failed...\n");
        exit(0);
    }
    else{
        printf("Server is listening..\n");
    }
    int start = clock();
    pthread_t tid[10];
    int i = 0;
    while (1) {
        
        addr_size = sizeof(serverStorage);
        connfd = accept(sockfd, (SA*)&serverStorage, &addr_size);
            func(connfd,newfile,servaddr, serverStorage,start);
            // Create writers thread
            if (pthread_create(&writerthreads[i++], NULL,
                               writer, &connfd)
                != 0)
 
                // Error in creating thread
                printf("Failed to create thread\n");
       
 
        if (i >= 50) {
            // Update i
            i = 0;
 
            while (i < 50) {
                // Suspend execution of
                // the calling thread
                // until the target
                // thread terminates
                pthread_join(writerthreads[i++],
                             NULL);
            }
            
            // Update i
            i = 0;
        }
        
        // if( pthread_create(&tid[i++], NULL, func(connfd, newfile, servaddr), &connfd) != 0 ){
        //     printf("server accept has failed...\n");
        //     exit(0);
        // }
 
        // // Creates a child process
        // if(i>=10){
        //     i=0;
        //     while(i<10){
        //         pthread_join(tid[i++], NULL);
        //     }
        //     exit(0);
        // }
    }

    fclose(newfile);
    //shutdown(sockfd, SHUT_RDWR);
   return 0;
}