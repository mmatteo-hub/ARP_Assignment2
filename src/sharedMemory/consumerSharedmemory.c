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


char dum[10];

int main(int argc, char *argv[]) {
  int dim = atoi(argv[1]);
  char B[dim];
  int shared_seg_size = (sizeof(B));
  char *ptr;
  int j;
  int shmfd = shm_open(SHMOBJ_PATH, O_RDONLY, 0666);
  ptr = mmap(NULL, shared_seg_size, PROT_READ, MAP_SHARED, shmfd, 0);
  sem_t * sem_id1 = sem_open(SEM_PATH_1, 0);
  sem_t * sem_id2 = sem_open(SEM_PATH_2, 0);

  // repeat 3 times
  for(j=0;j<3;j++) {
    // wait prod
    sem_wait(sem_id2);
    printf("1");
    /* get shared data */
    printf("%s\n", ptr);
    B = 
    //fflush(stdin);
    scanf("%s", B);
    printf("2");
    // start prod
    sem_post(sem_id1);
  }

  shm_unlink(SHMOBJ_PATH);
  sem_close(sem_id1);
  sem_close(sem_id2);
  sem_unlink(SEM_PATH_1);
  sem_unlink(SEM_PATH_2);

  return 0;
}
