#include "stdio.h"

enum numbers_1 { ZERO, ONE, TWO };

enum numbers_2 { FIVE = 5, TEN = 10, FIFTEEN = 15 };

enum numbers_3 {
  TWENTY_FIVE = 25,
  TWENTY_SIX,
  TWENTY_SEVEN,
  TWENTY_FIVE_2 = TWENTY_FIVE,
  TWENTY_SIX_2,
  TWENTY_SEVEN_2,
  TWENTY_FOUR = 24,
  MINUS_FOUR = -4,
  MINUS_THREE,
  MINUS_TWO
};

int main() {
  long long x;
  long long i = 0;
  i = 1;
  printf("%lld\n", i);
  x = ZERO;
  printf("%lld\n", x);
  i = 2;
  printf("%lld\n", i);
  x = ONE;
  printf("%lld\n", x);
  i = 3;
  printf("%lld\n", i);
  x = TWO;
  printf("%lld\n", x);
  i = 4;
  printf("%lld\n", i);
  x = FIVE;
  printf("%lld\n", x);
  i = 5;
  printf("%lld\n", i);
  x = TEN;
  printf("%lld\n", x);
  i = 6;
  printf("%lld\n", i);
  x = FIFTEEN;
  printf("%lld\n", x);
  i = 7;
  printf("%lld\n", i);
  x = TWENTY_FIVE;
  printf("%lld\n", x);
  i = 8;
  printf("%lld\n", i);
  x = TWENTY_SIX;
  printf("%lld\n", x);
  i = 9;
  printf("%lld\n", i);
  x = TWENTY_SEVEN;
  printf("%lld\n", x);
  i = 10;
  printf("%lld\n", i);
  x = TWENTY_FIVE_2;
  printf("%lld\n", x);
  i = 11;
  printf("%lld\n", i);
  x = TWENTY_SIX_2;
  printf("%lld\n", x);
  i = 12;
  printf("%lld\n", i);
  x = TWENTY_SEVEN_2;
  printf("%lld\n", x);
  i = 13;
  printf("%lld\n", i);
  x = TWENTY_FOUR;
  printf("%lld\n", x);
  i = 14;
  printf("%lld\n", i);
  x = MINUS_FOUR;
  printf("%lld\n", x);
  i = 15;
  printf("%lld\n", i);
  x = MINUS_THREE;
  printf("%lld\n", x);
  i = 16;
  printf("%lld\n", i);
  x = MINUS_TWO;
  printf("%lld\n", x);
}
