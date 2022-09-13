#include <stdio.h>

// void fu(int arr1[3][3], int arr[3]){
//   printf("ok");
// }

int main() {

  int n=3;

   int a[3][3][3] = {
     {{1,2,3},{4,5,6},{7,8,9}},
     {{10,11,12},{13,14,15},{16,17,18}},
     {{19,20,21},{22,23,24},{25,26,27}}
   };

   for(int i=0;i<n;i++)
     for(int j=0;j<n;j++)
       for(int k=0;k<n;k++) {
//          a[i][j][k]=i*n*n+j*n+k;
         printf("[%d,%d,%d]: %d\n", i, j, k, a[i][j][k]);
       }

//    int *x = a[2];
//    int *y = a[2][2];
//    fu(a[1], a[1][2]);

    return 0;

}
