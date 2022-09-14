#include "prefix_sum.h"

#include <math.h>

#include "helpers.h"
#include "strings.h"

struct thread_data_t {
  int32_t block_size;
  int *output;
  int (*scan_operator)(int, int, int);
  uint32_t n_loops;
  int32_t thread_id;
};

pthread_barrier_t barrier;


void *compute_prefix_sum(void *a) {
  prefix_sum_args_t *args = (prefix_sum_args_t *)a;

  int n = args->n_vals;
  int threads = args->n_threads;
  int thread_id = args->t_id;
  int * output = args->output_vals;


  for (int d = 2; d <= n; d <<=1) {
    pthread_barrier_wait(&barrier);
    for (int iteration = 0; ; iteration += 1) {
      int right =  (thread_id+(iteration*threads)+1)*d-1;
      if (right > n) break;
      int left = right - (d >> 1);
      printf("[%d] reduce: depth: %d, iteration: %d left: %d, right %d\n", thread_id, d, iteration,left,right);
      int prev = output[right];
      output[right] = args->op(output[left], output[right], args->n_loops);
      printf("[%d] reduce: depth: %d, left: %d [%d], right %d [%d], sum: %d\n\n", thread_id, d, left, output[left],right,prev, output[right]);
    }
  }

  int final = 0;
  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    std::cout << "After up sweep" << std::endl;
    for (int k = 0; k < n; k++) {
      std::cout << k << " : " << args->output_vals[k] << std::endl;
    }
    final = args->output_vals[n - 1];
    args->output_vals[n - 1] = 0;
    std::cout << "SUM:  " << final << std::endl;
  }

  for (int d = n; d >= 2; d >>=1) {
    pthread_barrier_wait(&barrier);
    for (int iteration = 0; ; iteration += 1) {
      int right =  (thread_id+(iteration*threads)+1)*d-1;
      if (right > n) break;
      int left = right - (d >> 1);
      int temp = output[left];
      output[left] = output[right];
      output[right] = args->op(temp, output[right], args->n_loops);
    }
  }

  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    for (int i = 0; i < n - 1; i++) {
      args->output_vals[i] = args->output_vals[i + 1];
    }
    args->output_vals[n - 1] = final;
  }


  return 0;
}