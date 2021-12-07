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

    #define BUFFSIZE 80

    #define KNRM  "\x1B[0m"
    #define KYEL  "\x1B[33m"

    #define USEC 1000

    #define CHECK(X) ({int __val = (X); (__val == -1 ? ({fprintf(stderr,"ERROR (" __FILE__ ":%d) -- %s\n",__LINE__,strerror(errno)); exit(-1);-1;}) : __val); })

    // defining pids for 5 processes
    int pid1, pid2, pid3, pid4, pid5, pid6, pid7, pid8;

    // defininf file pointer and a time variable to read the current date
    FILE *f;
    time_t clk;

    // function to start the execution of the processes, having a parameters list
    int spawn(const char * program, char ** arg_list) 
    {
        pid_t child_pid = fork();
        if (child_pid != 0)
            return child_pid;
        else 
        {
            execvp (program, arg_list);
            perror("exec failed");
            return 1;
        }
    }

    // main
    int main(int argc, char *argv[])
    {

        // opening the log file in writing mode to create if it does not exist
        f = fopen("./../log/logfile.txt","w");

        // defining all the paramters list for the processes
        char *arg_list_1[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/unnamedPipe", (char*)NULL};
        char *arg_list_2[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/producerNpipe", (char*)NULL};
        char *arg_list_3[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/producerScoket", (char*)NULL};
        char *arg_list_4[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/producerSharedmemory", (char*)NULL};
        char *arg_list_6[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/consumerNpipe", (char*)NULL};
        char *arg_list_7[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/consumerScoket", (char*)NULL};
        char *arg_list_8[] = {"/usr/bin/konsole", "--hold", "-e", "./../exc/consumerSharedmemory", (char*)NULL};

        char str[80];

        while(1)
        {
            printf("Type:\n1 to transfer data via UNNAMED PIPE\n2 to transfer data via NAMED PIPE\n3 to transfer data via SOCKET\n4 to transfer data via SHARED MEMOPRY\n0 to QUIT\n");
            fflush(stdout);

            scanf("%s",str);

            str[strcspn(str,"\n")] = 0;

            if(strlen(str) > 1) perror("Invalid input\n");
            else
            {
                switch (str[0])
                {
                    // unnamed pipe
                    case '1':
                        pid1 = spawn("/usr/bin/konsole", arg_list_1);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid1, ctime(&clk));
                        fflush(f);

                        pid5 = spawn("/usr/bin/konsole", arg_list_5);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid5, ctime(&clk));
                        fflush(f);

                        break;
                    
                    // named pipe
                    case '2':
                        pid2 = spawn("/usr/bin/konsole", arg_list_2);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid2, ctime(&clk));
                        fflush(f);

                        pid6 = spawn("/usr/bin/konsole", arg_list_6);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid6, ctime(&clk));
                        fflush(f);

                        break;

                    // socket
                    case '3':
                        pid3 = spawn("/usr/bin/konsole", arg_list_3);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid3, ctime(&clk));
                        fflush(f);

                        pid7= spawn("/usr/bin/konsole", arg_list_7);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid7, ctime(&clk));
                        fflush(f);

                        break;

                    // shared memory
                    case '4':
                        pid4 = spawn("/usr/bin/konsole", arg_list_4);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid4, ctime(&clk));
                        fflush(f);

                        pid8 = spawn("/usr/bin/konsole", arg_list_8);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"konsole (PID = %d) created at : %s", pid8, ctime(&clk));
                        fflush(f);

                        break;

                    // quit
                    case '0':
                        // writing on the log file
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"Master process (PID = %d) finished at : %s", (int)getpid(), ctime(&clk));
                        fflush(f);
                        fclose(f);

                        return 0;

                        break;

                    default:
                        printf("Invalid value\n");
                        fflush(stdout);
                        fseek(f,0,SEEK_END);
                        clk = time(NULL);
                        fprintf(f,"Master process (PID = %d) finished at : %s", (int)getpid(), ctime(&clk));
                        fflush(f);
                        break;
                }
            }

        }        
    }
