#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/time.h> 
#include <time.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    int sockfd2 = 0, n2 = 0;
    struct sockaddr_in serv_addr2;

    // time variable to compute the duration of the execution
    struct timeval end;
    end.tv_sec = 0;
    end.tv_usec = 0;
    // variable to store the total duration of the process
    double elapsed;
    double begin;

    char buff[80];

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
    
    int dim = atoi(argv[2]);
    char B[dim];

    memset(B, '0',sizeof(B));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while(n = read(sockfd, B, sizeof(B)-1) > 0);

    gettimeofday(&end,0);
    double time_final = end.tv_sec*1000000 + end.tv_usec;
    
    sleep(5);
    
    memset(buff, '0',sizeof(buff));
    if((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr2, '0', sizeof(serv_addr2)); 

    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_port = htons(5002); 

    if(inet_pton(AF_INET, argv[1], &serv_addr2.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd2, (struct sockaddr *)&serv_addr2, sizeof(serv_addr2)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while(n2 = read(sockfd2, buff, sizeof(buff)-1) > 0);
    begin = atof(buff);
    
    elapsed = time_final - begin;
    
    printf("Time socket = %f us\n", elapsed);
    fflush(stdout);

    /*while ( (n = read(sockfd, B, sizeof(B)-1)) > 0)
    {
        B[n] = 0;
        if(fputs(B, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }*/

    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}
