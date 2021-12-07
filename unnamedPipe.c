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

    int main(int argc, char * argv[])
    {
        // child pid
        pid_t child;  

        // dimension taken from master: already casted to length for the array of integers
        int dim = argv[1];

        child = fork(); // fork

        // child process: consumer
        if(!child)
        {
            int B[dim];
            for(int i=0; i<length(B);i++)
            {
                // filling the array
                B[i] = rand()%1000;
            }
            
        }
    }