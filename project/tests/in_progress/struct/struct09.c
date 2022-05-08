
//#include <stdio.h>
struct st {
  int a, b;
  struct st2{
    int x, y;
  } c;
} a = {1, 2, 3, 4};



struct st b = {1, 2, {3, 4}};

int main() {
  struct st2 c = {1, 2};
  return 0;
}