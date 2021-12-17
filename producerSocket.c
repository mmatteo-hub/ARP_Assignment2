#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <sys/time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int dim = atoi(argv[4]);
    printf("%i\n", dim);
    fflush(stdout);
	
    char A[dim];

    // time variable to compute the duration of the execution
    struct timeval begin,end;
    begin.tv_sec = 0;
    begin.tv_usec = 0;
    end.tv_sec = 0;
    end.tv_usec = 0;
    // variable to store the total duration of the process
    double time_init;
    char buff[80];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(A, '0', sizeof(A)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        gettimeofday(&begin,0);

        for(int i=0; i<dim;i++)
        {
            // filling the array
            A[i] = 'A' + (rand()%26);
        }
        write(connfd, A, strlen(A)-1);

        time_init = begin.tv_sec*1000000 + begin.tv_usec;

        sprintf(buff,"%f",time_init);

        sleep(3);

        write(connfd, buff, strlen(buff)-1);

        printf("begin = %f\n",time_init);
        fflush(stdout);

        close(connfd);
        sleep(1);
    }
}

