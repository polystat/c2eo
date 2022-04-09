//#include <stdio.h>
struct st1 {
  struct st2{
    int a;
    int b;
    int c;
  } a;
  int b;
  struct st2 c;
};

struct st1 obj1;
struct st2 obj2;

int main() {
  obj1.a.a = 8;
  return 0;
}
