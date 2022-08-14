#include <stdio.h>

double sqrt(double S) {
  double S2;//Variable for optimization. Number for square root calculation multiplied to 2
  double x; //Variable of axis x
  double c; //Correction of variable x

  //Initial value of x
  if (S > 1000000) {x = 1000;} else {
    if (S > 10000) {x = 100;} else {
      if (S > 100) {x = 10;} else {x = 1;}
    }
  }
  //S2 variable for optimization, we don't need multiply S to 2 every time
  S2 = S * 2;
  c = 0.0;
  while ( c > 1.0000001 || c < 0.9999999 ) { //Accuracy
    c = S2 / (S+x*x);
    x = x * c;
  }
  return x;
}

int main() {
  double S;
  S = 2.0;
  printf("%f\n", S);

  double result;
  result = sqrt(S);

  printf("%f\n", result);

  return 0;
}
