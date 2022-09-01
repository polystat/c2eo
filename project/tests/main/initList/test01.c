struct S1 {
  int a;
  int b;
  int c;
  int d;
};


struct S1 v = {1, .c=2, 3};

int a[5] = {1,2,[4]=5};

//#include <stdio.h>
int main()
{
//  printf("%d %d %d %d\n", v.a, v.b, v.c, v.d);
//  printf("%d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
  return 0;
}
