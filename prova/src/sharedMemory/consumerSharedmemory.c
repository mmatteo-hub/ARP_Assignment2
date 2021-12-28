#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH_1 "/sem_AOS_1"
#define SEM_PATH_2 "/sem_AOS_2"
#define SEM_PATH_3 "/sem_AOS_3"
#define SEM_PATH_4 "/sem_AOS_4"

// colours
#define KNRM  "\x1B[0m"
#define KCYN  "\x1B[36m"

#define MEGA 1048576
#define X 1024

int main(int argc, char *argv[])
{
	int dim = atoi(argv[1]);
    int dim_buff = dim*X;
    int number = dim*MEGA;
    char* B;
    B = (char *) malloc(sizeof(char)*number);
    if(B == NULL)
    	perror("Run out of memory\n");

    int shared_seg_size = (dim_buff);
    char *ptr;
    int j;
    struct timespec begin,end;
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_nsec = 0;
    end.tv_sec = 0;
    end.tv_nsec = 0;
    int shmfd = shm_open(SHMOBJ_PATH, O_RDONLY, 0666);
    ptr = mmap(NULL, shared_seg_size, PROT_READ, MAP_SHARED, shmfd, 0);
    sem_t * NotFull = sem_open(SEM_PATH_1, 0);
    sem_t * Mutex = sem_open(SEM_PATH_2, 0);
    sem_t * NotEmpty = sem_open(SEM_PATH_3, 0);
    sem_t * Sync = sem_open(SEM_PATH_4, 0);

    // getting the time
    clock_gettime(CLOCK_REALTIME,&begin);
    sem_post(Sync);
    int out = 0;
    for(int i = 0; i < number; i++)
    {
		sem_wait(NotEmpty);
		sem_wait(Mutex);
		B[i] = ptr[out];
		out = (out + 1) % dim_buff;
		sem_post(Mutex);
		sem_post(NotFull);
    }

    // getting the time
    clock_gettime(CLOCK_REALTIME,&end);

    double elapsed = (end.tv_sec - begin.tv_sec)*1000000000 + (end.tv_nsec - begin.tv_nsec);
    printf("\n%sTransfered %d MB(s) by SHARED MEMORY, time needed =  %lf ms%s\n\n", KCYN, dim, elapsed/1000000, KNRM);

    shm_unlink(SHMOBJ_PATH);
    sem_close(NotFull);
    sem_close(NotEmpty);
    sem_close(Mutex);
    sem_unlink(SEM_PATH_1);
    sem_unlink(SEM_PATH_2);
    sem_unlink(SEM_PATH_3);

    return 0;
}
