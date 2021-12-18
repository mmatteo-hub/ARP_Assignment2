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
    // defining variables for the socket
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int listenfd2 = 0, connfd2 = 0;
    struct sockaddr_in serv_addr2; 

    // dimension taken from master: already casted to length for the array of integers
    int dim = atoi(argv[1]);
	
    // initialising an array for the producer process
    char A[dim];

    // time variable to compute the duration of the execution
    struct timeval begin,end;
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_usec = 0;
    end.tv_sec = 0;
    end.tv_usec = 0;
    // variable to store the total duration of the process, once converted
    double time_init;
    char buff[80];

    // defininf the sockwt
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(A, '0', sizeof(A)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 
    
    listenfd2 = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr2, '0', sizeof(serv_addr2));
    memset(buff, '0', sizeof(buff)); 

    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr2.sin_port = htons(5002); 

    bind(listenfd2, (struct sockaddr*)&serv_addr2, sizeof(serv_addr2)); 

    listen(listenfd2, 10);

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        // getting the time
        gettimeofday(&begin,0);

        for(int i=0; i<dim;i++)
        {
            // filling the array randomly
            A[i] = 'A' + (rand()%26);
        }
        // writing on the socket
        write(connfd, A, strlen(A)-1);

        // converting the time and storing it into a variable
        time_init = begin.tv_sec*1000000 + begin.tv_usec;

        // printing on the buffer according to a specific format string
        sprintf(buff,"%f",time_init);

        // closing the socket
        close(connfd);
        connfd2 = accept(listenfd2, (struct sockaddr*)NULL, NULL); 
        
        // writing on the socket
        write(connfd2, buff, strlen(buff)-1);

        printf("begin = %f\n",time_init);
        fflush(stdout);

        // closing the socket
        close(connfd2);
        sleep(1);
    }
     
}

