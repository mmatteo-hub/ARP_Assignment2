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

        int fd = open(namedPipe,O_WRONLY);

        // dimension taken from master: already casted to length for the array of integers
        int dim = atoi(argv[1]);

        int A[dim];
        for(int i=0; i<sizeof(A);i++)
        {
            // filling the array
            A[i] = rand()%1000;
        }
        write(fd, A, sizeof(A));

        return 0;
    }