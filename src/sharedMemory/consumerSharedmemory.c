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
#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH_1 "/sem_AOS_1"
#define SEM_PATH_2 "/sem_AOS_2"
#define SEM_PATH_3 "/sem_AOS_3"
#define SEM_PATH_4 "/sem_AOS_4"


char dum[10];

int main(int argc, char *argv[]) {
  int dim = atoi(argv[1]);
  int dim_buff = dim/1024;
  char B[dim];
  int shared_seg_size = (dim_buff);
  char *ptr;
  int j;
  struct timeval begin,end;
    // initialising the variable for the time to the zero in order to avoid having rubbish data
    begin.tv_sec = 0;
    begin.tv_usec = 0;
    end.tv_sec = 0;
    end.tv_usec = 0;
  int shmfd = shm_open(SHMOBJ_PATH, O_RDONLY, 0666);
  ptr = mmap(NULL, shared_seg_size, PROT_READ, MAP_SHARED, shmfd, 0);
  sem_t * NotFull = sem_open(SEM_PATH_1, 0);
  sem_t * Mutex = sem_open(SEM_PATH_2, 0);
  sem_t * NotEmpty = sem_open(SEM_PATH_3, 0);
  sem_t * Sync = sem_open(SEM_PATH_4, 0);

  
  printf("Before consuming\n");
  fflush(stdout);
  // getting the time
  gettimeofday(&begin,0);
  sem_post(Sync);
  int out = 0;
  for(int i = 0; i < dim; i++)
  {
  	sem_wait(NotEmpty);
  	sem_wait(Mutex);
  	B[i] = ptr[out];
  	out = (out + 1) % dim_buff;
  	sem_post(Mutex);
  	sem_post(NotFull);
  }
  // getting the time
  gettimeofday(&end,0);

  double elapsed = (end.tv_sec - begin.tv_sec)*1000000 + (end.tv_usec - begin.tv_usec);
  printf("Time with shared memory: %lf us\n", elapsed);

  shm_unlink(SHMOBJ_PATH);
  sem_close(NotFull);
  sem_close(NotEmpty);
  sem_close(Mutex);
  sem_unlink(SEM_PATH_1);
  sem_unlink(SEM_PATH_2);
  sem_unlink(SEM_PATH_3);

  return 0;
}
