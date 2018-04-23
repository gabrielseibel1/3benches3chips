#include <stdio.h>
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
void *build_f_array(double *signal, int size);

/**
 * Builds an array of points with the function g
 */
void *build_g_array(double *signal, int size);

void print_array(double array[], int size, int id);

void build_empty_array(double *signal, int size);

void parse_signal_sizes(int argc, char *argv[]);

int sig1_size = 300;
int sig2_size = 300;
int conv_size = 300;

int main() {
  code double signal1[300];
  code double signal2[300];
  code double convolution[300]; /*pode ser code???*/

  //parse_signal_sizes(argc, argv);

  build_f_array(signal1, sig1_size);
  build_g_array(signal2, sig2_size);

  //print_array(signal1, sig1_size, 1 /*id for signal1*/);
  //print_array(signal2, sig2_size, 2 /*id for signal2*/);

  convolve(signal1, signal2, convolution);

  //print_array(convolution, conv_size, 3 /*id for conv*/);

  printf("Convolved an array of size %d and an array of size %d successfully.\n", sig1_size, sig2_size);

  return 0;
}

void convolve(double *signal1, double *signal2, double *out) {
  int i, j;
  for (i = 0; i < conv_size; ++i) { //iterate over elements of conv array
    out[i] = 0;
    for (j = 0; j < i && j < sig1_size; ++j) { //accumulate to get integral at each point
      out[i] += signal1[j] * signal2[i - j];
    }
  }
}

void print_array(double array[], int size, int id) {
  int i;
  printf("Signal%d: { ", id);
  for (i = 0; i < size; ++i) {
    printf(" %lf%s", array[i], (i + 1 < size) ? "," : "");
  }
  printf(" }\n");
}

void *build_f_array(double *signal, int size) {
  int i;
  for (i = 0; i < size; ++i) {
    signal[i] = f(i);
  }
}

void *build_g_array(double *signal, int size) {
  int i;
  for (i = 0; i < size; ++i) {
    signal[i] = g(i);
  }
}

double f(double t) {
  return t * t;
}

double g(double t) {
  return t;
}
