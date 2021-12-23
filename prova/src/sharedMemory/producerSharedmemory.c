#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#define SHMOBJ_PATH "/shm_AOS"
#define SEM_PATH_1 "/sem_AOS_1"
#define SEM_PATH_2 "/sem_AOS_2"
#define SEM_PATH_3 "/sem_AOS_3"

char * ptr;
int i, j;

int main(int argc, char *argv[]) {

  int dim = atoi(argv[1]);
  int dim_buff = dim/1024;
  printf("0.0");
  fflush(stdout);
  char A[dim];
  for(int i=0; i<dim;i++)
  {
    // filling the array
    A[i] = 'A' + (rand()%26);
  }
  printf("0.1");
  fflush(stdout);
  int shared_seg_size = (dim_buff);
  int shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, 0666);
  ftruncate(shmfd, shared_seg_size);
  ptr = mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  printf("0.2");
  fflush(stdout);
  sem_t * NotFull = sem_open(SEM_PATH_1, O_CREAT | O_RDWR, 0666, 1);
  sem_t * Mutex = sem_open(SEM_PATH_2, O_CREAT | O_RDWR, 0666, 1);
  sem_t * NotEmpty = sem_open(SEM_PATH_3, O_CREAT | O_RDWR, 0666, 1);
  sem_init(NotFull, 1, dim_buff); // initialized to 1
  sem_init(Mutex, 1, 0); // initialized to 0
  sem_init(NotEmpty, 1, 0); // initialized to 0

  printf("0.3");
  fflush(stdout);
  /*// put string into shmem
  for (i = 0; i < sizeof(A); ++i) {
    *ptr = A[i];
    ptr++;
  }
  printf("1.17777");

  // repeat 3 times
  for(j=0;j<3;j++) {
    sem_wait(sem_id1);
    /* start the consumer
    sem_post(sem_id2);
  }*/
  
  int in = 0;
  for(int i = 0; i < dim; i++)
  {
  	sem_wait(NotFull);
  	sem_wait(Mutex);
  	ptr[in] = A[i];
  	in = (in + 1) % dim_buff;
  	sem_post(Mutex);
  	sem_post(NotEmpty);
  	printf("w");
  	fflush(stdout);
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
