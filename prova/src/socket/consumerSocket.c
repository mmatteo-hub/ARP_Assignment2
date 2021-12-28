#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define MEGA 1048576
#define X 1024

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    int dim = atoi(argv[2]);
    int number = dim * MEGA;
    // number of kB
    int y = dim*X;
    
    // intialising the array for the consumer process
    char* B;
    B = (char *) malloc(sizeof(char)*number);
    if(B == NULL)
        perror("Run out of memory\n");

    char buffer[256];
    if (argc < 4) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    bzero(buffer,256);
    sprintf(buffer, "%s", "Ready");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(B, number);
    
    for(int i=0; i<y;i++)
    {
        // read from fd[0]
        n = read(sockfd, &B[i*X], sizeof(char)*X);
    }

    printf("Last 20 vals of B = %s\nDim(B)=%ld\n",&B[number-20],strlen(B));
    fflush(stdout);
    if (n < 0) 
         error("ERROR reading from socket");
    sprintf(buffer, "%s", "Done");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    close(sockfd);
    free(B);
    return 0;
}
