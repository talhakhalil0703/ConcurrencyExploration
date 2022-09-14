#include "spin_barrier.h"

void spin_barrier::init(int size) {
  N = size;
  sem_init(&arrivial, 0, 1);
  sem_init(&departure, 0, 0);
}

void spin_barrier::wait() {
  sem_wait(&arrivial);
  if (++counter < N) {
    sem_post(&arrivial);
  } else {
    sem_post(&departure);
  }

  sem_wait(&departure);
  if (--counter > 0) {
    sem_post(&departure);
  } else {
    sem_post(&arrivial);
  }
}
