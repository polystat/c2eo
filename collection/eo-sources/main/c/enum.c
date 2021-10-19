#include <stdio.h>
#include <stdlib.h>


enum week { Mon, Tue, Wed, Thur, Fri, Sat, Sun};

enum week currentDay = Sun;

enum week getSomeDay() {
    return currentDay;
}

int main(int argc, char** argv) {
  enum week someday, anotherDay;
  someday = Wed;
  printf("enum someday = %d\n", someday);
  anotherDay = getSomeDay();
  printf("enum another day = %d\n", anotherDay);
  return 0;
}
