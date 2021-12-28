#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <time.h> 
#include <sys/select.h>
#include <errno.h>

#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH_1 "/sem_AOS_1"
#define SEM_PATH_2 "/sem_AOS_2"
#define SEM_PATH_3 "/sem_AOS_3"
#define SEM_PATH_4 "/sem_AOS_4"

#define MEGA 1048576
#define X 1024

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


char * ptr;
int i, j;

int main(int argc, char *argv[])
{
	int dim = atoi(argv[1]);
	// dimension incread by a factor 4 to be compared
	int dim_buff = dim*4*X;
	int number = dim*MEGA;

	// opening the log file in append mode to write on it
    f = fopen("./../log/logfile.txt","a");
  
  	char* A;
	A = (char *) malloc(sizeof(char)*number);
	if(A == NULL)
	{
    	perror("Run out of memory\n");
		fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER SHARED MEM0RY: Run out of memory at : %s", ctime(&clk));
        fflush(f);
	}
	else
    {
        fseek(f,0,SEEK_END);
        clk = time(NULL);
        fprintf(f,"PRODUCER SHARED MEM0RY: Allocated %d MB of memory at : %s",atoi(argv[1]), ctime(&clk));
        fflush(f);
    }

	for(int i=0; i<number;i++)
	{
		// filling the array randomly
		A[i] = 'A' + (rand()%26);
	}

	int shared_seg_size = (dim_buff);
	int shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, 0666);
	ftruncate(shmfd, shared_seg_size);
	ptr = mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	
	sem_t * NotFull = sem_open(SEM_PATH_1, O_CREAT | O_RDWR, 0666, 1);
	sem_t * Mutex = sem_open(SEM_PATH_2, O_CREAT | O_RDWR, 0666, 1);
	sem_t * NotEmpty = sem_open(SEM_PATH_3, O_CREAT | O_RDWR, 0666, 1);
	sem_t * Sync = sem_open(SEM_PATH_4, O_CREAT | O_RDWR, 0666, 1);
	sem_init(NotFull, 1, dim_buff); // initialized to 1
	sem_init(Mutex, 1, 1); // initialized to 0
	sem_init(NotEmpty, 1, 0); // initialized to 0
	sem_init(Sync, 1, 0); // initialized to 0
	
	int in = 0;
	sem_wait(Sync);
	for(int i = 0; i < number; i++)
	{
		sem_wait(NotFull);
		sem_wait(Mutex);
		ptr[in] = A[i];
		in = (in + 1) % dim_buff;
		sem_post(Mutex);
		sem_post(NotEmpty);
	}
  
	shm_unlink(SHMOBJ_PATH);
	sem_close(NotFull);
	sem_close(Mutex);
	sem_close(NotEmpty);
	sem_unlink(SEM_PATH_1);
	sem_unlink(SEM_PATH_2);
	sem_unlink(SEM_PATH_3);
	munmap(ptr, shared_seg_size);

	return 0;
}
