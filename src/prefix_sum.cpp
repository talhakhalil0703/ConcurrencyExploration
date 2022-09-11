#include "prefix_sum.h"

#include <math.h>

#include "helpers.h"
#include "strings.h"

static void up_sweep(void *a);
static void down_sweep(void *a);

void *compute_prefix_sum(void *a) {
  up_sweep(a);
  down_sweep(a);
  return 0;
}

static void up_sweep(void *a) {
  // The first thing to do is compute the addition of an even index of the array
  // with the value to the right of it On subsequent runs the addition is now
  // computed 2^depth as the offset only of the array, we can pretend the array
  // has basically been halfed but the addition still only occurs for the 'even'
  // indices of this subsequent run.
  // This is done until the depth comes to an end.

  prefix_sum_args_t *args = (prefix_sum_args_t *)a;
  int *output = args->output_vals;
  int n = args->n_vals;
  int (*scan_operator)(int, int, int) = args->op;

  // What is the depth, the depth is the log(n), where n is the number of values
  double depth = log2(n - 1);
  // int offset = 0;

  for (int d = 0; d < depth; d++) {
    int two_d_1 = pow(2, d + 1);
    int two_d = pow(2, d);
    for (int k = 0; k < n-1; k += two_d_1) {
      output[k + two_d_1 - 1] = scan_operator(
          output[k + two_d - 1], output[k + two_d_1 - 1], args->n_loops);
      std::cout << k + two_d_1 - 1 << ":" << output[k + two_d_1 - 1]
                << std::endl;
    }
  }
}

static void down_sweep(void *a) {
  prefix_sum_args_t *args = (prefix_sum_args_t *)a;
  int *output = args->output_vals;
  int n = args->n_vals;
  int (*scan_operator)(int, int, int) = args->op;
  int final_value = output[n-1];
  output[n -1] = 0;
  double depth = log2(n - 1);
  int temp=0;

  for (int d = depth; d >= 0; d--){
    int two_d_1 = pow(2, d + 1);
    int two_d = pow(2, d);
    for (int k = 0; k < n-1; k+= two_d_1){
      temp = output[k + two_d -1];
      output[k+two_d-1] = output[k+two_d_1-1];
      output[k+two_d_1-1] = scan_operator(temp, output[k+two_d_1-1], args->n_loops);
      std::cout << k + two_d_1 - 1 << ":" << output[k + two_d_1 - 1] << std::endl;
    }
  }


  //Making it exclusive by shifting the array as well as appending the final sum to the end
  for (int i =0; i < n-1; i++){
    output[i] = output[i+1];
  }
  output[n-1] = final_value;

}
