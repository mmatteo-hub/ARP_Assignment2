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
        char * timeCompute = "/tmp/timeCompute";

        printf("Inside consumer for Named Pipe\n\n");
        fflush(stdout);

        // time variable to compute the duration of the execution
        struct timeval begin,end;
        // initialising the variable for the time to the zero in order to avoid having rubbish data
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process, once converted
        double elapsed;
        // initialising the array for storing the time duration
        char el[10];

        // opening the pipe
        int fd = open(namedPipe,O_RDONLY);
        int fdt = open(timeCompute,O_RDONLY);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        // getting the time
        gettimeofday(&begin,0);

        // intialising the array for the consumer process
        char B[dim];

        // reading from the pipe
        read(fd, B, sizeof(B));

        // closing the pipe
        close(fd);

        // getting the time
        gettimeofday(&end,0);
        
        // reading from the pipe
        read(fdt, &el, sizeof(el));
        // doing a scanf to store the value read into the array according to a specific format string
        sscanf(el,"%lf",&elapsed);

        // closing the pipe
        close(fdt);

        // checking that both begin and end time are non zero values
        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            // converting the time into micro seconds and storing in into a variable
            elapsed += (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            
            printf("Duration for transfering data by named pipe: %lf usec\n",elapsed);
            fflush(stdout);
        }

        return 0;
    }
