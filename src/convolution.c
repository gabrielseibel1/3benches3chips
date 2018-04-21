#include <stdio.h>
#include <memory.h>

#define SIZE1 6
#define SIZE2 3

void convolve(double *signal1, double *signal2, double *out);

double f(double t);

double g(double t);

void *build_f_array(double *signal, size_t size);

void * build_g_array(double *signal, size_t size);

void print_array(double array[], size_t size, int id) ;

void build_empty_array(double *signal, size_t size) ;

size_t conv_size = (SIZE1 < SIZE2 ? SIZE2 : SIZE1);

int main() {
  double signal1[SIZE1], signal2[SIZE2], convolution[conv_size];

  build_f_array(signal1, SIZE1);
  build_g_array(signal2, SIZE2);
  build_empty_array(convolution, conv_size);

  print_array(signal1, SIZE1, 1);
  print_array(signal2, SIZE2, 2);

  convolve(signal1, signal2, convolution);

  print_array(convolution, conv_size, 3);

  return 0;
}

void convolve(double *signal1, double *signal2, double *out) {
  for (size_t i = 0; i < conv_size; ++i) { //iterate over elements of conv
    out[i] = 0;
    for (size_t j = 0; j < i && j < SIZE1; ++j) {
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

void *build_g_array(double *signal, size_t size) {
  for (int i = 0; i < size; ++i) {
    signal[i] = g(i);
  }
}

void *build_f_array(double *signal, size_t size) {
  for (int i = 0; i < size; ++i) {
    signal[i] = f(i);
  }
}

double f(double t) {
  return t*t; /* signal value at time i */
}

double g(double t) {
  return t; /* signal value at time i */
}