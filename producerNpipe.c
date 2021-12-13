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
        // named pipe
        char * namedPipe = "/tmp/namedPipe";
        char * timeCompute = "/tmp/timeCompute";

        // time variable to compute the duration of the execution
        struct timeval begin,end;
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process
        double elapsed;

        int fd = open(namedPipe,O_WRONLY);
        int fdt = open(timeCompute,O_WRONLY);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        gettimeofday(&begin,0);

        int A[dim];
        for(int i=0; i<dim;i++)
        {
            // filling the array
            A[i] = rand()%1000;
        }
        write(fd, A, sizeof(A));
        close(fd);
        
        gettimeofday(&end,0);
        
        char el[10];

        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            sprintf(el,"%f",elapsed);
            write(fdt,el,sizeof(el));
        }
        close(fdt);

        return 0;
    }