#include "io.h"
#include "helpers.h"

void read_file(struct options_t* args,
               int*              n_vals,
               int**             input_vals,
               int**             output_vals) {

  	// Open file
	std::ifstream in;
	in.open(args->in_file);
	// Get num vals
	in >> *n_vals;

	// Alloc input and output arrays
	*input_vals = (int*) malloc(*n_vals * sizeof(int));
	*output_vals = (*input_vals);

	// Read input vals
	for (int i = 0; i < *n_vals; ++i) {
		in >> (*input_vals)[i];
	}
}

void write_file(struct options_t*         args,
               	struct prefix_sum_args_t* opts) {
  // Open file
	std::ofstream out;
	out.open(args->out_file, std::ofstream::trunc);

	// Write solution to output file
	for (int i = 0; i < opts->n_vals; ++i) {
		out << opts->output_vals[i] << std::endl;
	}

	out.flush();
	out.close();

	// Free memory
	free(opts->input_vals);
	// free(opts->output_vals);
}
