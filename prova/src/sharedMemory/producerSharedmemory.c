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

char * ptr;
int i, j;

int main(int argc, char *argv[]) {

  int dim = atoi(argv[1]);
  char A[dim];
  for(int i=0; i<dim;i++)
  {
    // filling the array
    A[i] = 'A' + (rand()%26);
  }
  int shared_seg_size = (sizeof(A));
  int shmfd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, 0666);
  ftruncate(shmfd, shared_seg_size);
  ptr = mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  sem_t * sem_id1= sem_open(SEM_PATH_1, O_CREAT | O_RDWR, 0666, 1);
  sem_t * sem_id2= sem_open(SEM_PATH_2, O_CREAT | O_RDWR, 0666, 1);
  sem_init(sem_id1, 1, 1); // initialized to 1
  sem_init(sem_id2, 1, 0); // initialized to 0

  // put string into shmem
  for (i = 0; i < strlen(A); ++i) {
    *ptr = A[i];
    ptr++;
  }

  // repeat 3 times
  for(j=0;j<3;j++) {
    sem_wait(sem_id1);
    /* start the consumer */
    sem_post(sem_id2);
  }

  shm_unlink(SHMOBJ_PATH);
  sem_close(sem_id1);
  sem_close(sem_id2);
  sem_unlink(SEM_PATH_1);
  sem_unlink(SEM_PATH_2);
  munmap(ptr, shared_seg_size);

  return 0;
}
