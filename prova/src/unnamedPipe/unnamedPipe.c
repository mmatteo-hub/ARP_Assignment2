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
        struct timeval begin,end;
        // initialising the variable for the time to the zero in order to avoid having rubbish data
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
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
	int x = 1024;
	int y = dim*1024;
	number = y*x;

        

        // fork
        pid = fork();
        // child process: consumer
        if(!pid)
        {
            printf("Unnamed Pipe started\n");
            fflush(stdout);
	    close(fd[1]);
            // initialising the array for the consumer process
            char* B;
            B = (char *) malloc(sizeof(char)*number);
            if(B == NULL)
            	perror("Run out of memory\n");
            
            for(int i=0; i<y;i++)
            {
		    // read from fd[0]
		    read(fd[0], &B[i*x], sizeof(char)*x);
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
            gettimeofday(&begin,0);
  	    for(int i=0; i<y;i++)
            {
		    // write on the fd[1]
		    write(fd[1], &A[i+x], sizeof(char)*x);
	    }
	    close(fd[1]);
	    wait(NULL);
	    // taking the final time
            gettimeofday(&end,0);
	    
        }

        // checking that both begin and end time are non zero values
        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            // converting the time into micro seconds and storing it into the variable
            elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);

            printf("Duration for transfering %d Mb by unnamed pipe: %lf usec\n", dim, elapsed);
            fflush(stdout);
        }
        //free(A);
        //free(B);

        return 0;
    }
