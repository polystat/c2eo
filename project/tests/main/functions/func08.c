#include <stdio.h>

typedef long long i64;

i64 fun2(i64 a, i64 b);

i64 fun1(i64 a, i64 b) {
  i64 result = a + b;
  result = fun2(result, 8);
  return result;
}

i64 fun2(i64 a, i64 b) {
  i64 result = a + b;
  return result;
}

int main() {
    i64 res = fun1(7, 9);
    printf("%lld\n", res);
}