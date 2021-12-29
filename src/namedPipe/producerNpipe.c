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
#include <sys/select.h>

#define MEGA 1048576
#define X 1024

// colours
#define KNRM  "\x1B[0m"
#define KGRN  "\x1B[32m"

// time variable to compute the duration of the execution
struct timespec begin,end;
int ready = 0;

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


void endtime(int sig_number)
{
	if(sig_number == SIGUSR1)
	{
		// getting the time
        clock_gettime(CLOCK_REALTIME,&end);
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER NAMED PIPE: Received signal SIGUSR1 at : %s", ctime(&clk));
        fflush(f);
	}
}
void consumer_ready(int sig_number)
{
	if(sig_number == SIGUSR2)
	{
		ready = 1;
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER NAMED PIPE: Received signal SIGUSR2 at : %s", ctime(&clk));
        fflush(f);
	}
}

int main(int argc, char * argv[])
{   
    // initialising named pipe already created into the master process
    char * namedPipe = "/tmp/namedPipe";

    // opening the log file in append mode to write on it
    f = fopen("./../log/logfile.txt","a");

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
    {
        perror("Run out of memory\n");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER NAMED PIPE: Run out of memory at : %s", ctime(&clk));
        fflush(f);
    }
    else
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER NAMED PIPE: Allocated %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
        fflush(f);
    }

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

    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"PRODUCER NAMED PIPE: Written correctly %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
    fflush(f);

    free(A);
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"PRODUCER NAMED PIPE: Released %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
    fflush(f);

    // closing the pipe
    close(fd);
    
    while(end.tv_sec == 0 && end.tv_nsec == 0);
    
    elapsed = (end.tv_sec*1000000000 + end.tv_nsec) - (begin.tv_sec*1000000000 + begin.tv_nsec);
        
    printf("%s\nTransfered %d MB(s) by NAMED PIPE, time needed =  %lf ms%s\n\n", KGRN, dim, elapsed/1000000, KNRM);
    fflush(stdout);

    return 0;
}
