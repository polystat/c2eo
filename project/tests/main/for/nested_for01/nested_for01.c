#include "stdio.h"

int main() {
  for (long long i=0; i<3; i++)
  {
    printf("%d\n",i);
    for (long long j=0; j<3; j++)
    {
      printf("%d\n",j);
      for (long long k=0; k<3; k++)
      {
        printf("%d\n",k);
      }
    }
  }
  return 0;
}
