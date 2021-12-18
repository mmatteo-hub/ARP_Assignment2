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

        // time variable to compute the duration of the execution
        struct timeval begin,end;
        // initialising the variable for the time to the zero in order to avoid having rubbish data
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process, once converted
        double elapsed;

        // opening the pipe
        int fd = open(namedPipe,O_WRONLY);
        int fdt = open(timeCompute,O_WRONLY);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        // initialising the array for the producer process
        char A[dim];
        for(int i=0; i<dim;i++)
        {
            // filling the array randomly
            A[i] = 'A' + (rand()%26);
        }
        // getting the time
        gettimeofday(&begin,0);

        // writing on the pipe
        write(fd, A, sizeof(A));
        // closing the pipe
        close(fd);
        
        // getting the time
        gettimeofday(&end,0);
        
        // initialising the array to pass the time duration
        char el[10];

        // checking botj begin and end time are non zero values
        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            // converting the time into micro seconds and storing it into the variable
            elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            // printing on the array with a specific forma type
            sprintf(el,"%f",elapsed);
            // writing on the pipe
            write(fdt,el,sizeof(el));
        }
        // closing the pipe
        close(fdt);

        return 0;
    }
