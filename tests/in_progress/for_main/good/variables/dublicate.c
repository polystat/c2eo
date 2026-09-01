#include <stdio.h>

int a;
int b;
int c;
int d;
int e;
int f;

int e_;
int f_;

int e_;
int f_;

int a;
int b;

int main() {
  a = b = c = 5;
  d = e = 10;
  e_ = f_ = 13;

  printf("a,b,b = %d,%d,%d\n",a,b,c);
  printf("d,e = %d,%d\n",d,e);
  printf("e_,f_ = %d,%d\n",e_,f_);

  return 0;
}
