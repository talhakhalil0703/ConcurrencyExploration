#include "helpers.h"
#include "math.h"

static int32_t get_power_of_2_input(int n_vals, int * outputs);

prefix_sum_args_t* alloc_args(int n_threads) {
  return (prefix_sum_args_t*) malloc(n_threads * sizeof(prefix_sum_args_t));
}

int next_power_of_two(int x) {
    int pow = 1;
    while (pow < x) {
        pow *= 2;
    }
    return pow;
}

void fill_args(prefix_sum_args_t *args,
               int n_threads,
               int n_vals,
               int *inputs,
               int *outputs,
               bool spin,
               int (*op)(int, int, int),
               int n_loops) {
    int32_t new_size = get_power_of_2_input(n_vals, outputs);
    for (int i = 0; i < n_threads; ++i) {
        args[i] = {inputs, outputs, spin, n_vals,
                   n_threads, i, op, n_loops, 0, new_size};
    }
}

static int32_t get_power_of_2_input(int n_vals, int * outputs){
    int _log2n = log2(n_vals);
    if ((1 << _log2n) == n_vals) return n_vals;

    int new_val = 1 << (_log2n+1);

    outputs = (int *)realloc(outputs, (new_val*sizeof(int)));

    for (int i = n_vals; i < new_val; i++){
        outputs[i] = 0;
    }
    return new_val;
}