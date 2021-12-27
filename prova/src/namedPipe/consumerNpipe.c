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
        // initialising named pipe already created into the master process
        char * namedPipe = "/tmp/namedPipe";

        printf("Inside consumer for Named Pipe\n\n");
        fflush(stdout);

        // opening the pipe
        int fd = open(namedPipe,O_RDONLY);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);
        int mega = 1024 * 1024;
	int number = dim * mega;
	int x = 1024;
	int y = dim*1024;
	number = y*x;
	pid_t pid_prod = atoi(argv[2]);

        // intialising the array for the consumer process
        char* B;
            B = (char *) malloc(sizeof(char)*number);
            if(B == NULL)
            	perror("Run out of memory\n");
        // send signal
        kill(pid_prod, SIGUSR2);
        // reading from the pipe
        for(int i=0; i<y;i++)
            {
		    // read from fd[0]
		    read(fd, &B[i*x], sizeof(char)*x);
	    }

        // send signal
        kill(pid_prod, SIGUSR1);
        // closing the pipe
        close(fd);
       
        return 0;
    }
