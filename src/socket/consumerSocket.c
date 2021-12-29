#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <time.h> 
#include <sys/select.h>
#include <errno.h>

#define MEGA 1048576
#define X 1024

// defining file pointer and a time variable to read the current date
FILE *f;
time_t clk;

// function to detect an error and write it into the log file
int check (int retval)
{
    if(retval == -1)
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"ERROR("__FILE__") -- %s at %s",strerror(errno), ctime(&clk));
        exit(-1);
    }
    return retval;
}

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

    // opening the log file in append mode to write on it
    f = fopen("./../log/logfile.txt","a");
    
    int dim = atoi(argv[2]);
    int number = dim * MEGA;
    // number of kB
    int y = dim*X;
    
    // intialising the array for the consumer process
    char* B;
    B = (char *) malloc(sizeof(char)*number);
    if(B == NULL)
    {
    	perror("Run out of memory\n");
		fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Run out of memory at : %s", ctime(&clk));
        fflush(f);
	}
    else
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Allocated %d MB of memory at : %s",atoi(argv[2]), ctime(&clk));
        fflush(f);
    }

    char buffer[256];
    if (argc < 4)
    {
       	fprintf(stderr,"usage %s hostname port\n", argv[0]);
       	fseek(f,0,SEEK_END);
    	clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Usage %s hostname port at : %s", argv[0], ctime(&clk));
        fflush(f);
       	exit(0);
    }
    portno = atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("ERROR opening socket");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Error opening the socket at : %s", ctime(&clk));
        fflush(f);
    }
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr,"ERROR, no such host\n");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: No host found at : %s", ctime(&clk));
        fflush(f);
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) error("ERROR connecting");
    bzero(buffer,256);
    sprintf(buffer, "%s", "Ready");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
	{
		error("ERROR writing to socket");
		fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Error writing to socket at : %s", ctime(&clk));
        fflush(f);
	}

    bzero(B, number);
    
    for(int i=0; i<y;i++)
    {
        // read from fd[0]
        n = read(sockfd, &B[i*X], sizeof(char)*X);
    }

    if (n < 0)
	{
		error("ERROR reading from socket");
		fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Error reading from socket at : %s", ctime(&clk));
        fflush(f);
	}
    else
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Read correctly %d MB of memory at : %s",atoi(argv[2]), ctime(&clk));
        fflush(f);
    }

    sprintf(buffer, "%s", "Done");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
	{
		error("ERROR writing to socket");
		fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER SOCKET: Error writing to socket at : %s", ctime(&clk));
        fflush(f);
	}

    close(sockfd);
    free(B);
	
    return 0;
}
