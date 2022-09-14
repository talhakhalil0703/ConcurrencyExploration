#include "prefix_sum.h"

#include <math.h>

#include "helpers.h"
#include "strings.h"

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
      output[right] = args->op(output[left], output[right], args->n_loops);
    }
  }

  int final = 0;
  pthread_barrier_wait(&barrier);

  if (thread_id == 0) {
    final = args->output_vals[n - 1];
    args->output_vals[n - 1] = 0;
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