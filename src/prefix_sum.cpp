#include "prefix_sum.h"

#include <math.h>

#include "helpers.h"
#include "strings.h"

struct thread_data_t {
  int32_t block_size;
  int *output;
  int (*scan_operator)(int, int, int);
  uint32_t n_loops;
};

pthread_barrier_t barrier;

static int *intermediate_array;
static void up_sweep(thread_data_t *a);
static void down_sweep(thread_data_t *a);
static void perform_increment(thread_data_t *t, int32_t value);
static void prefix_scan(thread_data_t *t);

void *compute_prefix_sum(void *a) {
  prefix_sum_args_t *args = (prefix_sum_args_t *)a;

  int block_size = args->block_size;
  // Needs a start value, end value, needs pointer to data, needs thread id,
  // needs operator
  thread_data_t thread_data;
  // thread_data.starting_index = args->t_id * block_size;
  thread_data.block_size = block_size;
  thread_data.n_loops = args->n_loops;
  thread_data.output = &(args->output_vals[args->t_id * block_size]);
  thread_data.scan_operator = args->op;
  prefix_scan(&thread_data);

  if (pthread_barrier_wait(&barrier) != 0) {
    std::cout << "Intermediate Array" << std::endl;

    // Take all scans and now perform aggergate the sum of each block
    intermediate_array = (int *)malloc(args->n_threads * sizeof(int));
    for (int i = 0; i < args->n_threads; i++) {
      // From output and block size we should know where these sums are
      // located and can create a new array
      intermediate_array[i] = args->output_vals[(i + 1) * block_size - 1];
    }
    // Perform prefix scan on intermediate array that is shared by everyone
    // here
    thread_data_t intermediate_prefix_scan;
    intermediate_prefix_scan.output = intermediate_array;
    intermediate_prefix_scan.block_size = args->n_threads;
    intermediate_prefix_scan.scan_operator = args->op;
    intermediate_prefix_scan.n_loops = args->n_loops;
    // intermediate_prefix_scan.starting_index = 0;
    prefix_scan(&intermediate_prefix_scan);
  }
  pthread_barrier_wait(&barrier);

  // perform addition per thread
  if (args->t_id > 0) {
    perform_increment(&thread_data, intermediate_array[args->t_id - 1]);
  }

  // barrier wait here
  if (pthread_barrier_wait(&barrier) != 0) {
    // free intermediate array memory
    free(intermediate_array);
  }
  pthread_barrier_wait(&barrier);

  return 0;
}

static void prefix_scan(thread_data_t *thread_data) {
  std::cout << "Before Scan" << std::endl;
  // for (int k = 0; k < thread_data->block_size + thread_data->starting_index -1; k++) {
  //   std::cout  " : " << k << " : " << thread_data->output[k] << std::endl;
  // }
  up_sweep(thread_data);
  down_sweep(thread_data);

  // std::cout << "After Scan" << std::endl;
  // for (int k = thread_data->starting_index; k < thread_data->block_size + thread_data->starting_index -1; k++) {
  //   std::cout << thread_data->starting_index << " : " << k << " : " << thread_data->output[k] << std::endl;
  // }
}

static void up_sweep(thread_data_t *thread_data) {
  // The first thing to do is compute the addition of an even index of the array
  // with the value to the right of it On subsequent runs the addition is now
  // computed 2^depth as the offset only of the array, we can pretend the array
  // has basically been halfed but the addition still only occurs for the 'even'
  // indices of this subsequent run.
  // This is done until the depth comes to an end.

  int *output = thread_data->output;
  int n = thread_data->block_size;
  int (*scan_operator)(int, int, int) = thread_data->scan_operator;

  // What is the depth, the depth is the log(n), where n is the number of values
  // This has the possiblity of being wrong it could be log2(n)-1, unclear in
  // the paper because of formatting issues
  double depth = log2(n - 1);

  for (int d = 0; d < depth; d++) {
    int two_d_1 = pow(2, d + 1);
    int two_d = pow(2, d);
    for (int k = 0; k < n - 1; k += two_d_1) {
      output[k + two_d_1 - 1] = scan_operator(output[k + two_d - 1], output[k + two_d_1 - 1], thread_data->n_loops);
      std::cout << "Up sweep: " << d << " " << k + two_d_1 - 1 << ":" << output[k + two_d_1 - 1] << std::endl;
    }
  }
}

static void down_sweep(thread_data_t *thread_data) {
  int *output = thread_data->output;
  int n = thread_data->block_size;
  int (*scan_operator)(int, int, int) = thread_data->scan_operator;

  int final_value = output[n - 1];
  output[n - 1] = 0;
  double depth = log2(n - 1);
  int temp = 0;

  for (int d = depth; d >= 0; d--) {
    int two_d_1 = pow(2, d + 1);
    int two_d = pow(2, d);
    for (int k = 0; k < n - 1; k += two_d_1) {
      temp = output[k + two_d - 1];
      output[k + two_d - 1] = output[k + two_d_1 - 1];
      output[k + two_d_1 - 1] = scan_operator(temp, output[k + two_d_1 - 1], thread_data->n_loops);
      std::cout << "Down sweep: " << d << " " << k + two_d_1 - 1 << ":" << output[k + two_d_1 - 1] << std::endl;
    }
  }

  // Making it exclusive by shifting the array as well as appending the final
  // sum to the end
  for (int i = 0; i < n - 1; i++) {
    output[i] = output[i + 1];
  }
  output[n - 1] = final_value;
}

static void perform_increment(thread_data_t *thread_data, int32_t value) {
  for (int i = 0; i < thread_data->block_size; i++) {
    thread_data->output[i] = thread_data->output[i] + value;
  }
}
