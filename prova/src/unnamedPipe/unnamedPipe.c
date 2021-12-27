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

    int main(int argc, char * argv[])
    {
        // fork pid
        pid_t pid; 

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
        	perror("Not created pipe");
        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);
	    int mega = 1024 * 1024;
	    int number = dim * mega;

        // fork
        pid = fork();
        // child process: consumer
        if(!pid)
        {
            printf("Inside consumer for Unnamed Pipe\n\n");
            fflush(stdout);
	        close(fd[1]);
            // initialising the array for the consumer process
            char* B;
            B = (char *) malloc(sizeof(char)*number);
            if(B == NULL)
            	perror("Run out of memory\n");

            for(int i=0; i<number;i++)
            {
                // read from fd[0]
                read(fd[0], &B[i], sizeof(char));
            }
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
            	perror("Run out of memory\n");
            for(int i=0; i<number;i++)
            {
                // filling the array randomly
                A[i] = 'A' + (rand()%26);
            }
	        // taking the initial time
            clock_gettime(CLOCK_REALTIME,&begin);
  	        for(int i=0; i<number;i++)
            {
                // write on the fd[1]
                write(fd[1], &A[i], sizeof(char));
	        }
	        close(fd[1]);
	        wait(NULL);
	        // taking the final time
            clock_gettime(CLOCK_REALTIME,&end);
	    
        }

        // checking that both begin and end time are non zero values
        if((begin.tv_sec != 0 || begin.tv_nsec != 0) && (end.tv_sec != 0 || end.tv_nsec != 0))
        {
            // converting the time into micro seconds and storing it into the variable
            elapsed = (end.tv_sec - begin.tv_sec)*1000000000 + (end.tv_nsec - begin.tv_nsec);

            printf("Duration for transfering data by unnamed pipe: %lf msec\n",elapsed/1000000);
            fflush(stdout);
        }
        //free(A);
        //free(B);

        return 0;
    }
