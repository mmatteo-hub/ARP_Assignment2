#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/time.h> 
#include <time.h> 

#define MEGA 1048576
#define X 1024

int main(int argc, char *argv[])
{
    // 
    int sockfd = 0, n = 0;
    struct sockaddr_in serv_addr;

    if(argc != 4)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }
    
    // dimension taken from master: already casted to length for the array of integers
    int dim = atoi(argv[2]);
    int number = dim * MEGA;
    // number of kB
    int y = dim*X;
    // get pid producer process
    pid_t pid_prod = atoi(argv[3]);

    // intialising the array for the consumer process
    char* B;
    B = (char *) malloc(sizeof(char)*number);
    if(B == NULL)
        perror("Run out of memory\n");
       
    // send signal
    kill(pid_prod, SIGUSR2);
    

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
       kill(pid_prod, SIGKILL);
       return 1;
    }
    
    // send signal
    kill(pid_prod, SIGUSR2);
    
    for(int i=0; i<y;i++)
    {
        // read from fd[0]
        read(sockfd, &B[i*X], sizeof(char)*X);
    }

    close(sockfd);
    
    // send signal
    kill(pid_prod, SIGUSR1);
    free(B);

    return 0;
}
