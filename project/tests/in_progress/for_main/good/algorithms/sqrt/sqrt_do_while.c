#include <stdio.h>

double sqrt(double S) {
  double S2;//Variable for optimization. Number for square root calculation multiplied to 2
  double x; //Variable of axis x
  double c; //Correction of variable x
  int i; //Generation index

  i = 1;

  //Initial value of x
  if (S > 1000000) {x = 1000;} else {
    if (S > 10000) {x = 100;} else {
      if (S > 100) {x = 10;} else {x = 1;}
    }
  }
  //S2 variable for optimization, we don't need multiply S to 2 every time
  S2 = S * 2;

  do {
    c = S2 / (S+x*x);
    x = x * c;
    // Возможно разное число итераций по сравнению с EO.
    // Стоит ли это считать нарушением семантики?
    // Нужно ил анализировать промежуточные вычисления?
//     printf("  Generation %d, x = %f", i++, x);
//     printf(". Correction c = %f\n", c);
  } while ( c > 1.0000001 || c < 0.9999999 ); //Accuracy

  return x;
}


int main() {
    double S;
  //     printf("Square root calculator\nPlease input a number: ");
//     scanf("%lf", &S);

    S = 2.0;
    double result;
    result = sqrt(S);

    printf("Square root of %f is %f", S, result);

    return 0;
}
