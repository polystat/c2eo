int test() {
  struct T {
    int x;
    int y;
  } s1;
  s1.x = 1;
  s1.y = 2;
  {
    struct T {
      int y;
      int x;
    } s2;
    s2.y = 1;
    s2.x = 2;
    if (s1.x - s2.y != 0)
      return 1;
    if (s1.y - s2.x != 0)
      return 2;
  }
  return 0;
}

// #include <stdio.h>
int main() {
  int x = test();
  //	printf("%d\n", x);
  return x;
}
