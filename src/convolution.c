#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

/**
 * Convolves two signals and writes output to a third array
 */
void convolve(double *signal1, double *signal2, double *out);

/**
 * Returns the value a function f at time t
 */
double f(double t);

/**
 * Returns the value of a function g at time t
 */
double g(double t);

/**
 * Builds an array of points with the function f
 */
void *build_f_array(double *signal, size_t size);

/**
 * Builds an array of points with the function g
 */
void *build_g_array(double *signal, size_t size);

void print_array(double array[], size_t size, int id);

void build_empty_array(double *signal, size_t size);

void parse_signal_sizes(int argc, char *argv[]);

size_t sig1_size;
size_t sig2_size;
size_t conv_size;

int main(int argc, char *argv[]) {
  parse_signal_sizes(argc, argv);

  double signal1[sig1_size], signal2[sig2_size], convolution[conv_size];

  build_f_array(signal1, sig1_size);
  build_g_array(signal2, sig2_size);
  build_empty_array(convolution, conv_size);

  //print_array(signal1, sig1_size, 1 /*id for signal1*/);
  //print_array(signal2, sig2_size, 2 /*id for signal2*/);

  convolve(signal1, signal2, convolution);

  //print_array(convolution, conv_size, 3 /*id for conv*/);

  printf("Convolved an array of size %d and an array of size %d successfully.\n", sig1_size, sig2_size);

  return 0;
}

void parse_signal_sizes(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Expected args: size_of_signal_1 size_of_signal_2\n");
    exit(EXIT_FAILURE);
  }

  sig1_size = (size_t) atoi(argv[1]);
  sig2_size = (size_t) atoi(argv[2]);
  conv_size = (sig1_size < sig2_size ? sig2_size : sig1_size); //pick the largest size
}

void convolve(double *signal1, double *signal2, double *out) {
  for (size_t i = 0; i < conv_size; ++i) { //iterate over elements of conv array
    out[i] = 0;
    for (size_t j = 0; j < i && j < sig1_size; ++j) { //accumulate to get integral at each point
      out[i] += signal1[j] * signal2[i - j];
    }
  }
}

void print_array(double array[], size_t size, int id) {
  printf("Signal%d: { ", id);
  for (int i = 0; i < size; ++i) {
    printf(" %lf%s", array[i], (i + 1 < size) ? "," : "");
  }
  printf(" }\n");
}

void build_empty_array(double *signal, size_t size) {
  memset(signal, 0, size);
}

void *build_f_array(double *signal, size_t size) {
  for (int i = 0; i < size; ++i) {
    signal[i] = f(i);
  }
}

void *build_g_array(double *signal, size_t size) {
  for (int i = 0; i < size; ++i) {
    signal[i] = g(i);
  }
}

double f(double t) {
  return t * t;
}

double g(double t) {
  return t;
}