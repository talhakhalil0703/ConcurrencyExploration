#ifndef _SPIN_BARRIER_H
#define _SPIN_BARRIER_H

#include <pthread.h>
#include <iostream>
#include <semaphore.h>
#include <atomic>
class spin_barrier {
  sem_t arrivial;
  sem_t departure;
  std::atomic<int> counter = 0;
  int N ;

 public:
  void init(int);
  void wait();
};

#endif
