#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <time.h>

// colours
#define KNRM  "\x1B[0m"
#define KYEL  "\x1B[33m"

#define MEGA 1048576
#define X 1024

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    // time variable to compute the duration of the execution
    struct timespec begin,end;
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    // variable to store the total duration of the process, once converted
    double elapsed;
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 3)
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
    // dimension taken from master
    int dim = atoi(argv[1]);
    int number = dim * MEGA;
    // number kB
    int y = dim*X;
	
    // initialising the array for the producer process
    char* A;
    A = (char *) malloc(sizeof(char)*number);
    if(A == NULL)
        perror("Run out of memory\n");

    for(int i=0; i<number;i++)
    {
        // filling the array randomly
        A[i] = 'A' + (rand()%26);
    }
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error("ERROR on binding");

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	if (newsockfd < 0) 
		error("ERROR on accept");
	
	bzero(buffer,256);
	
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	
	// getting the time
	clock_gettime(CLOCK_REALTIME,&begin);
	// writing on the socket
	for(int i=0; i<y;i++)
	{
		// write on the fd[1]
		n = write(newsockfd, &A[i*X], sizeof(char)*X);
	}
	if (n < 0) error("ERROR writing to socket");
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	
	if (n < 0) error("ERROR reading from socket");
	
	// getting the time
	clock_gettime(CLOCK_REALTIME,&end);
	elapsed = (end.tv_sec*1000000000 + end.tv_nsec) - (begin.tv_sec*1000000000 + begin.tv_nsec);
	printf("\n%sTransfered %d MB(s) by SOCKET, time needed =  %lf ms%s\n\n", KYEL, dim, elapsed/1000000, KNRM);
	fflush(stdout);
	
	close(newsockfd);
	
	return 0; 
}
