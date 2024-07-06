// #include <stdio.h>
// #include <netdb.h>
// #include <netinet/in.h>
// #include <netinet/ip.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <pthread.h>
// #include <semaphore.h>
// #define MAX 4000
// #define PORT 9090
// #define SA struct sockaddr

// void* func(){
//     int sockfd, connfd, clientfd;
//     struct sockaddr_in servaddr;
//     char buffer[1024]={0};
 
//     // socket creation and verification checking
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1) {
//         printf("socket creation failed...\n");
//         exit(0);
//     }
//     bzero(&servaddr, sizeof(servaddr));
 
//     // assigning IP, PORT, that can be seen in data.txt
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     servaddr.sin_port = htons(PORT);
 
//     // connecting the client socket to server socket
//     if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0){
//         printf("connection with the server failed...\n");
//         exit(0);
//     }
 
//     char numbers[5];
//     int a = rand()%10;
//     sprintf(numbers,"%d", a);
//     send(sockfd, numbers, strlen(numbers)+1, 0);
//     //printf("Request %d from client sent.\n", a);
//     read(sockfd, buffer, 1024);
//     printf("Factorial is received from server: %s\n", buffer);
//     a++;
//     close(clientfd);
// }

// int main()
// {
//     //using threading to run the above function
//     pthread_t threaded_arr[10];
//     for(int i=0; i<10;i++){
//         int a=i;
//         pthread_create(&threaded_arr[i], NULL, func, NULL);
//     }
//     for (size_t i=0; i<10; i++)
//     {
//         pthread_join(threaded_arr[i], NULL);
//     }
    
//     return 0;
// }
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
#include <pthread.h>
#include <semaphore.h>
#define MAX 4000
#define PORT 9090
#define SA struct sockaddr

void* func(){
    int sockfd, connfd, clientfd;
    struct sockaddr_in servaddr;
    char buffer[1024]={0};
 
    // socket creation and verification checking
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    bzero(&servaddr, sizeof(servaddr));
 
    // assigning IP, PORT, that can be seen in data.txt
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
 
    // connecting the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))!= 0){
        printf("connection with the server failed...\n");
        exit(0);
    }
    for(int i=1; i<=20; i++){
        char numbers[5];
        int a = rand()%10;
        //printf("hello");
        sprintf(numbers,"%d", i);
        send(sockfd, numbers, strlen(numbers)+1, 0);
        //printf("Request %d from client sent.\n", a);
        read(sockfd, buffer, 1024);
        printf("Factorial is received from server: %s\n", buffer);
        a++;
        // close(clientfd);
    }
    //close(clientfd);
}

int main()
{
    //using threading to run the above function
    pthread_t threaded_arr[10];
    for(int i=0; i<10;i++){
        int a=i;
        pthread_create(&threaded_arr[i], NULL, func, NULL);
    }
    for (size_t i=0; i<10; i++)
    {
        pthread_join(threaded_arr[i], NULL);
    }
    return 0;
}