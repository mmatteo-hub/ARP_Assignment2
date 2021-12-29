#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h>
#include <sys/wait.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <time.h> 
#include <errno.h>
#include <math.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/select.h>

#define MEGA 1048576
#define X 1024

//colours
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"

// defining file pointer and a time variable to read the current date
FILE *f;
time_t clk;

int main(int argc, char * argv[])
{
    // fork pid
    pid_t pid; 

    // opening the log file in append mode to write on it
    f = fopen("./../log/logfile.txt","a");

    // time variable to compute the duration of the execution
    struct timespec begin,end;
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    // variable to store the total duration of the process, once converted
    double elapsed;

    // file descriptor for pipe
    int fd[2];
    if(pipe(fd)<0)
    {
        perror("Not created pipe");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"UNNAMED PIPE: Pipe not created at : %s", ctime(&clk));
        fflush(f);
    }
    // dimension taken from master: already casted to length for the array of integers
    int dim = atoi(argv[1]);
    int number = dim * MEGA;
    // number of kB
    int y = dim*X;

    // fork
    pid = fork();
    // child process: consumer
    if(!pid)
    {
        close(fd[1]);
        // initialising the array for the consumer process
        char* B;
        B = (char *) malloc(sizeof(char)*number);
        if(B == NULL)
        {
            perror("Run out of memory\n");
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"CONSUMER UNNAMED PIPE: Run out of memory at : %s", ctime(&clk));
            fflush(f);
        }
        else
        {
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"CONSUMER UNNAMED PIPE: Allocated %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
            fflush(f);
        }
        
        for(int i=0; i<y;i++)
        {
            // read from fd[0]
            read(fd[0], &B[i*X], sizeof(char)*X);
        }

        free(B);
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"CONSUMER UNNAMED PIPE: Released %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
        fflush(f);

        close(fd[0]);
        exit(0);
    }
    // father process: producer
    else if(pid)
    {
        // initialising the array for the producer process
        char* A;
        close(fd[0]);
        A = (char *) malloc(sizeof(char)*number);
        if(A == NULL)
        {
            perror("Run out of memory\n");
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"PRODUCER UNNAMED PIPE: Run out of memory at : %s", ctime(&clk));
            fflush(f);
        }
        else
        {
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"PRODUCER UNNAMED PIPE: Allocated %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
            fflush(f);
        }

        for(int i=0; i<number;i++)
        {
            // filling the array randomly
            A[i] = 'A' + (rand()%26);
        }
        
        // taking the initial time
        clock_gettime(CLOCK_REALTIME,&begin);
        int c = 0;
        for(int i=0; i<y;i++)
        {
            // write on the fd[1]
            write(fd[1], &A[i*X], sizeof(char)*X);
            c++;
        }

        free(A);
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER UNNAMED PIPE: Released %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
        fflush(f);

        close(fd[1]);
        wait(NULL);
        // taking the final time
        clock_gettime(CLOCK_REALTIME,&end);
    }

    // checking that both begin and end time are non zero values
    if((begin.tv_sec != 0 || begin.tv_nsec != 0) && (end.tv_sec != 0 || end.tv_nsec != 0))
    {
        // converting the time into micro seconds and storing it into the variable
        elapsed = (end.tv_sec*1000000000 + end.tv_nsec) - (begin.tv_sec*1000000000 + begin.tv_nsec);

        printf("%s\nTransfered %d MB(s) by UNNAMED PIPE, time needed =  %lf ms%s\n\n", KRED, dim, elapsed/1000000,KNRM);
        fflush(stdout);
    }

    return 0;
}
