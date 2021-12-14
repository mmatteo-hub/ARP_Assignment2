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

        printf("Inside consumer for Named Pipe\n\n");
        fflush(stdout);

        // time variable to compute the duration of the execution
        struct timeval begin,end;
        begin.tv_sec = 0;
        begin.tv_usec = 0;
        end.tv_sec = 0;
        end.tv_usec = 0;
        // variable to store the total duration of the process
        double elapsed;
        char el[10];

        int fd = open(namedPipe,O_RDONLY);
        int fdt = open(timeCompute,O_RDONLY);

        read(fdt, &el, sizeof(el));
        sscanf(el,"%lf",&elapsed);

        close(fdt);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        gettimeofday(&begin,0);

        char B[dim];

        read(fd, B, sizeof(B));

        close(fd);

        gettimeofday(&end,0);

        if((begin.tv_sec != 0 || begin.tv_usec != 0) && (end.tv_sec != 0 || end.tv_usec != 0))
        {
            elapsed += (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
            printf("Duration for transfering data by named pipe: %lf usec\n",elapsed);
            fflush(stdout);
        }

        return 0;
    }