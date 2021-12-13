#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;

    int A[atoi(argv[1])];
    for(int i=0; i < atoi(argv[1]); i++)
    {
        A[i] = rand()%1000;
    }

    struct sockaddr_in serv_addr, cli_addr;

    int n;
    if (argc < 3)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
     
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
     
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     
    if (newsockfd < 0)
        error("ERROR on accept");

    bzero(A,atoi(argv[1]));

    n = write(newsockfd,A,sizeof(A));
    if (n < 0)
        error("ERROR writing to socket");

    return 0; 
}