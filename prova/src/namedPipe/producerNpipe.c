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

// time variable to compute the duration of the execution
struct timeval begin,end;
int ready = 0;

void endtime(int sig_number)
{
	if(sig_number == SIGUSR1)
	{
		// getting the time
        	gettimeofday(&end,0);
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
        // initialising named pipe already created into the master process
        char * namedPipe = "/tmp/namedPipe";

        // initialising the variable for the time to the zero in order to avoid having rubbish data
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process, once converted
        double elapsed;
        
        signal(SIGUSR1, endtime);
        signal(SIGUSR2, consumer_ready);

        // opening the pipe
        int fd = open(namedPipe,O_WRONLY);

        // dimension taken from master
        int dim = atoi(argv[1]);
        int mega = 1024 * 1024;
	int number = dim * mega;
	int x = 1024;
	int y = dim*1024;
	number = y*x;

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
        gettimeofday(&begin,0);

        // writing on the pipe
        for(int i=0; i<y;i++)
        {
        	// write on the fd[1]
		write(fd, &A[i*x], sizeof(char)*x);
	}
        // closing the pipe
        close(fd);
        
        while(end.tv_sec == 0 && end.tv_usec == 0);
        
        elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            
            printf("Duration for transfering %d Mb by named pipe: %lf usec\n", dim, elapsed);
            fflush(stdout);

        return 0;
    }
