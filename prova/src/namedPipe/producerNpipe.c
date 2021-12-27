#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <time.h> 
#include <errno.h>
#include <math.h>
#include <sys/time.h>

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

int main(int argc, char * argv[])
{
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    
    // initialising named pipe already created into the master process
    char * namedPipe = "/tmp/namedPipe";

    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    // variable to store the total duration of the process, once converted
    double elapsed;
    
    signal(SIGUSR1, endtime);
    signal(SIGUSR2, consumer_ready);

    // opening the pipe
    int fd = open(namedPipe,O_WRONLY);

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
	while(!ready);

    // getting the time
    clock_gettime(CLOCK_REALTIME,&begin);

    // writing on the pipe
    for(int i=0; i<y;i++)
    {
        // write on the fd[1]
        write(fd, &A[i*X], sizeof(char)*X);
    }
    free(A);
    // closing the pipe
    close(fd);
    
    while(end.tv_sec == 0 && end.tv_nsec == 0);
    
    elapsed = (end.tv_sec*1000000000 + end.tv_nsec) - (begin.tv_sec*1000000000 + begin.tv_nsec);
        
    printf("Duration for transfering %d MB by named pipe: %lf ms\n", dim, elapsed/1000000);
    fflush(stdout);

    return 0;
}
