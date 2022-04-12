//#include "stdio.h"

int getPos(int x) {
  x *= 2;
  return x;
}

int arr[10];
int main() {
    arr[getPos(3)] = 19;
    //  printf("%d\n", obj1.a);
    arr[getPos(3)];
    return 0;
}
