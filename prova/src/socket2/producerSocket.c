#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h> 
#include <sys/time.h> 

// colours
#define KNRM  "\x1B[0m"
#define KYEL  "\x1B[33m"

#define MEGA 1048576
#define X 1024

// time variable to compute the duration of the execution
struct timespec begin,end;
int ready = 0;

void endtime(int sig_number)
{
	if(sig_number == SIGUSR1)
	{
		// getting the time
        clock_gettime(CLOCK_REALTIME,&end);
	}
}
void consumer_ready(int sig_number)
{
	if(sig_number == SIGUSR2)
	{
		ready = 1;
	}
}

int main(int argc, char *argv[])
{
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    // variable to store the total duration of the process, once converted
    double elapsed;
    
    signal(SIGUSR1, endtime);
    signal(SIGUSR2, consumer_ready);
    
    // defining variables for the socket
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;  

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

    printf("Last 20 vals of A = %s\nDim(A)=%ld\n",&A[number-20],strlen(A));
    fflush(stdout);
    
    // defininf the sockwt
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(A, '0', sizeof(A)); 
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        //while(!ready);

        // getting the time
        clock_gettime(CLOCK_REALTIME,&begin);

        // writing on the socket
        for(int i=0; i<y;i++)
        {
            // write on the fd[1]
            write(connfd, &A[i*X], sizeof(char)*X);
        }
        // closing the socket
        close(connfd);
        
        while(end.tv_sec == 0 && end.tv_nsec == 0);

        elapsed = (end.tv_sec*1000000000 + end.tv_nsec) - (begin.tv_sec*1000000000 + begin.tv_nsec);
        printf("\n%sTransfered %d MB(s) by SOCKET, time needed =  %lf ms%s\n\n", KYEL, dim, elapsed/1000000, KNRM);
        fflush(stdout);

        return 0;
    }
}

