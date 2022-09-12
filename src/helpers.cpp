#include "helpers.h"

static int32_t get_block_size(int n_threads, int n_vals, int * outputs);

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
    int32_t block_size = get_block_size(n_threads, n_vals, outputs);
    for (int i = 0; i < n_threads; ++i) {
        args[i] = {inputs, outputs, spin, n_vals,
                   n_threads, i, op, n_loops, block_size};
    }
}

static int32_t get_block_size(int n_threads, int n_vals, int * outputs){
    int32_t block_remainder = n_vals%n_threads;
    if (block_remainder == 0){
        return n_vals/n_threads;
    }
    std::cout << "APPENDING INPUT" << std::endl;
    int32_t block_size = n_vals/n_threads + 1;
    int32_t new_array_size = n_vals + (n_threads - block_remainder);
    outputs = (int *)realloc(outputs, (new_array_size*sizeof(int)));
    for (int i = n_vals; i < new_array_size; i++){
        outputs[i] = 0;
    }
    return block_size;
}