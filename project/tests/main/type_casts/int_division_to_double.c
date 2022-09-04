#include <stdio.h>

int sum = 17, count = 5;
double mean;

int main() {
  mean = (double) sum / count;
  printf("%f\n", mean);
}