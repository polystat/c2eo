
//#include <stdio.h>
struct st {
  int a;
  struct st2 {
    int x, y;
  } b;
} a/* = {1, {2, 3}}*/;



struct st2 b = {1,2};

int main() {

  return 0;
}