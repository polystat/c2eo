
//#include <stdio.h>
struct st {
  int a;
  struct st2 {
    int x, y;
  } b;
} a/* = {1, {2, 3}}*/;



struct st2 b = {1,2};

//struct st c = {1,{2,3}};

//int arr[3] = {1,2,3};

int main() {
  struct st2 b2 = {1,2};
//  int arr2[3] = {1,2,3};
  b.x = 1;
  b.y = 2;
//
//  c.a = 1;
//  c.b.x = 2;
//  c.b.y = 3;

  return 0;
}