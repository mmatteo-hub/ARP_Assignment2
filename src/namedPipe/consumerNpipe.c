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

int main(int argc, char * argv[])
{
    // initialising named pipe already created into the master process
    char * namedPipe = "/tmp/namedPipe";

    // opening the log file in append mode to write on it
    f = fopen("./../log/logfile.txt","a");

    // opening the pipe
    int fd = open(namedPipe,O_RDONLY);

    // dimension taken from master: already casted to length for the array of integers
    int dim = atoi(argv[1]);
    int number = dim * MEGA;
    // number of kB
    int y = dim*X;
    
    pid_t pid_prod = atoi(argv[2]);

    // intialising the array for the consumer process
    char* B;
    B = (char *) malloc(sizeof(char)*number);
    if(B == NULL)
    {
        perror("Run out of memory\n");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER NAMED PIPE: Run out of memory at : %s", ctime(&clk));
        fflush(f);
    }
    else
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER NAMED PIPE: Allocated %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
        fflush(f);
    }
       
    // send signal
    check(kill(pid_prod, SIGUSR2));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"CONSUMER NAMED PIPE: Sent signal SIGUSR2 to the producer (PID = %d) at : %s", pid_prod, ctime(&clk));
    fflush(f);

    // reading from the pipe
    for(int i=0; i<y;i++)
    {
        // read from fd[0]
        read(fd, &B[i*X], sizeof(char)*X);
    }

    // send signal
    check(kill(pid_prod, SIGUSR1));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"CONSUMER NAMED PIPE: Sent signal SIGUSR1 to the producer (PID = %d) at : %s", pid_prod, ctime(&clk));
    fflush(f);

    free(B);
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"CONSUMER NAMED PIPE: Released %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
    fflush(f);

    // closing the pipe
    close(fd);
    
    return 0;
}
