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


char dum[10];

int main(int argc, char *argv[]) {
  int dim = atoi(argv[1]);
  int dim_buff = dim/1024;
  char B[dim];
  int shared_seg_size = (dim_buff);
  char *ptr;
  int j;
  printf("111");
  fflush(stdout);
  int shmfd = shm_open(SHMOBJ_PATH, O_RDONLY, 0666);
  ptr = mmap(NULL, shared_seg_size, PROT_READ, MAP_SHARED, shmfd, 0);
  sem_t * NotFull = sem_open(SEM_PATH_1, 0);
  sem_t * Mutex = sem_open(SEM_PATH_2, 0);
  sem_t * NotEmpty = sem_open(SEM_PATH_3, 0);

  /*// repeat 3 times
  for(j=0;j<3;j++) {
    // wait prod
    sem_wait(sem_id2);
    printf("111");
    fflush(stdout);
    /* get shared data
    printf("%s\n", ptr);
    fflush(stdout);
    scanf("%s", B);
    printf("222");
    printf("%s\n", B);
    fflush(stdout);
    // start prod
    sem_post(sem_id1);
  }*/
  
  printf("Before consuming\n");
  fflush(stdout);
  int out = 0;
  for(int i = 0; i < dim; i++)
  {
  	sem_wait(NotEmpty);
  	sem_wait(Mutex);
  	B[i] = ptr[out];
  	out = (out + 1) % dim_buff;
  	sem_post(Mutex);
  	sem_post(NotFull);
  	printf("r");
  	fflush(stdout);
  }
  
  printf("%s", B);
  fflush(stdout);

  shm_unlink(SHMOBJ_PATH);
  sem_close(NotFull);
  sem_close(NotEmpty);
  sem_close(Mutex);
  sem_unlink(SEM_PATH_1);
  sem_unlink(SEM_PATH_2);
  sem_unlink(SEM_PATH_3);

  return 0;
}
