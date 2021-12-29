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
#include <sys/select.h>

#define BUFFSIZE 80

#define KNRM  "\x1B[0m"
#define KYEL  "\x1B[33m"

#define USEC 1000

// colours
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// defining pids for 5 processes
int pid1, pid2, pid3, pid4, pid6, pid7, pid8;

// defining file pointer and a time variable to read the current date
FILE *f;
time_t clk;

// function to detect an error and write it into the log file
int check (int retval)
{
    if(retval == -1)
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"ERROR("__FILE__") -- %s at %s",strerror(errno), ctime(&clk));
        exit(-1);
    }
    return retval;
}

// function to kill al processes and respective write on the log file all actions computed
void kill_processes()
{
    check(kill(pid1,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid1, ctime(&clk));
    fflush(f);

    check(kill(pid2,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid2, ctime(&clk));
    fflush(f);

    check(kill(pid3,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid3, ctime(&clk));
    fflush(f);

    check(kill(pid4,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid4, ctime(&clk));
    fflush(f);

    check(kill(pid6,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid6, ctime(&clk));
    fflush(f);

    check(kill(pid7,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid7, ctime(&clk));
    fflush(f);

    check(kill(pid8,SIGKILL));
    fseek(f,0,SEEK_END);
    clk = time(NULL);
    fprintf(f,"MASTER: Process (PID = %d) killed at : %s", pid8, ctime(&clk));
    fflush(f);
}

// function to start the execution of the processes, having a parameters list
int spawn(const char * program, char ** arg_list) 
{
    pid_t child_pid = fork();
    if (child_pid != 0)
        return child_pid;
    else 
    {
        check(execvp (program, arg_list));
        perror("exec failed");
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"MASTER: Exec failed at : %s", ctime(&clk));
        fflush(f);
        return 1;
    }
}

// main
int main(int argc, char *argv[])
{
    // initialising named pipe
    char * namedPipe = "/tmp/namedPipe";
    
    // creating pipes
    mkfifo(namedPipe,0666);
    
    // variables for socket port
    char portstr[5];
    int port;

    // opening the log file in writing mode to create if it does not exist
    f = fopen("./../log/logfile.txt","w");

    // variables for receiving the dimension from the user
    int flag1;
    int dimension;

    flag1 = 0;
    while(!flag1)
    {
        printf("%sEnter number of MB to trasfer (maximum 100MB): ", KNRM);
        fflush(stdout);
        check(scanf("%d",&dimension));
        if(dimension > 0 && dimension <= 100) flag1 = 1;
        else
        {
            printf("%sInvalid dimension.\n", KWHT);
            fflush(stdout);
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"MASTER: Invalid dimension (%d) received at : %s", dimension, ctime(&clk));
            fflush(f);
        }
    }
    char strToPass[10];
    check(sprintf(strToPass, "%d", dimension));

    // defining all the paramters list for the processes
    char *arg_list_1[] = {"./../exe/unnamedPipe", strToPass, (char*)NULL};
    char *arg_list_2[] = {"./../exe/producerNpipe", strToPass, (char*)NULL};
    char *arg_list_4[] = {"./../exe/producerSharedmemory", strToPass, (char*)NULL};
    char *arg_list_8[] = {"./../exe/consumerSharedmemory", strToPass, (char*)NULL};

    char str[80];

    printf("%s%d MB to pass", KNRM, atoi(strToPass));
    fflush(stdout);
    printf("\n%sType:\n%s1 to transfer data via UNNAMED PIPE\n%s2 to transfer data via NAMED PIPE\n%s3 to transfer data via SOCKET\n%s4 to transfer data via SHARED MEMORY\n%s0 to QUIT %s\n", KNRM, KRED, KGRN, KYEL, KCYN, KWHT, KNRM);
    fflush(stdout);

    while(1)
    {
        // take input from the user
        check(scanf("%s",str));

        str[strcspn(str,"\n")] = 0;

        if(strlen(str) > 1)
        {
            perror("Invalid input\n");
            fseek(f,0,SEEK_END);
            clk = time(NULL);
            fprintf(f,"MASTER: Invalid input menù received at : %s", ctime(&clk));
            fflush(f);
        }
        else
        {
            switch (str[0])
            {
                // unnamed pipe
                case '1':
                    
                    // producer and consumer
                    pid1 = spawn("./../exe/unnamedPipe", arg_list_1);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid1, ctime(&clk));
                    fflush(f);

                    break;
                
                // named pipe
                case '2':
                    
                    // producer
                    pid2 = spawn("./../exe/producerNpipe", arg_list_2);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid2, ctime(&clk));
                    fflush(f);

                    // consumer
                    char prod_pid[10];
                    check(sprintf(prod_pid, "%d", pid2));
                    char *arg_list_6[] = {"./../exe/consumerNpipe", strToPass, prod_pid, (char*)NULL};
                    pid6 = spawn("./../exe/consumerNpipe", arg_list_6);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid6, ctime(&clk));
                    fflush(f);

                    break;

                // socket
                case '3':
                    
                    // producer
                    port = 5000 + (rand()%100); //random port chosen
                    check(sprintf(portstr, "%d", port));
                    char *arg_list_3[] = {"./../exe/producerSocket", strToPass, portstr, (char*)NULL};
                    pid3 = spawn("./../exe/producerSocket", arg_list_3);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid3, ctime(&clk));
                    fflush(f);
                    
                    sleep(5);
                    
                    // consumer
                    char *arg_list_7[] = {"./../exe/consumerSocket", "127.0.0.1", strToPass, portstr, (char*)NULL};
                    pid7= spawn("./../exe/consumerSocket", arg_list_7);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid7, ctime(&clk));
                    fflush(f);

                    break;

                // shared memory
                case '4':
                    
                    // producer
                    pid4 = spawn("./../exe/producerSharedmemory", arg_list_4);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid4, ctime(&clk));
                    fflush(f);

                    sleep(2);
                    // consumer
                    pid8 = spawn("./../exe/consumerSharedmemory", arg_list_8);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: process (PID = %d) created at : %s", pid8, ctime(&clk));
                    fflush(f);

                    break;

                // quit
                case '0':
                    // writing on the log file
                    kill_processes();
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                   
                    // unlink named pipes
		     check(unlink(namedPipe));
		     fseek(f,0,SEEK_END);
		     clk = time(NULL);
		     fprintf(f,"MASTER: Unlinked named pipe at : %s", ctime(&clk));
		     fflush(f);
		     fprintf(f,"MASTER: Master process (PID = %d) finished at : %s", (int)getpid(), ctime(&clk));
                    fflush(f);
		     check(fclose(f));

                    return 0;

                    break;

                default:
                    printf("Invalid value\n");
                    fflush(stdout);
                    fseek(f,0,SEEK_END);
                    clk = time(NULL);
                    fprintf(f,"MASTER: Invalid value at : %s", ctime(&clk));
                    fflush(f);
                    break;
            }
        }
    }
    
}
