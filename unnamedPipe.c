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

    int main(int argc, char * argv[])
    {
        // fork pid
        pid_t pid;  

        // time variable to compute the duration of the execution
        struct timeval begin,end;
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process
        double elapsed;

        // file descriptor for pipe
        int fd[2];
        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        int flag = 0;

        // taking the initial time
        gettimeofday(&begin,0);

        // fork
        pid = fork();
        // child process: consumer
        if(!pid)
        {
            int B[dim];
            
            // read from fd[0]
            read(fd[0], &B, sizeof(B));
            // taking the final time
            gettimeofday(&end,0);
        }
        // father process: producer
        else if(pid)
        {
            int A[dim];
            for(int i=0; i<dim;i++)
            {
                // filling the array
                A[i] = rand()%1000;
            }

            // write on the fd[1]
            write(fd[1], &A, sizeof(A));
        }

        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            printf("Duration for transfering data by unnamed pipe: %lf usec\n",elapsed);
            fflush(stdout);
        }

        return 0;
    }