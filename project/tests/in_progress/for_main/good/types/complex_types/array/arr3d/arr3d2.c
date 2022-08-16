#include <stdio.h>

// void fu(int arr1[3][3], int arr[3]){
//   printf("ok");
// }

int main() {

  int n=3;

   int a[3][3][3];

   for(int i=0;i<n;i++)
     for(int j=0;j<n;j++)
       for(int k=0;k<n;k++) {
         a[i][j][k]=i*n*n+j*n+k;
       }

//    int *x = a[2];
//    int *y = a[2][2];
//    fu(a[1], a[1][2]);

    return 0;

}
