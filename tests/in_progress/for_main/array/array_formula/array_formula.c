#include <stdio.h>

// Вычисление одномерного индекса для многомерного массива
int Index(int rank, int*  bounds, int* koord) {
    int result_index = 0;
    for(int i = 1; i < rank; ++i) {
        result_index += bounds[i] * koord[i-1];
    }
    result_index += koord[rank - 1];
    return result_index;
}

int Address(int index, int type_size) {
    return index * type_size;
}

int A1[6] = {0, 1, 2, 3, 4, 5};
int bounds1[1] = {6};

int A2[3][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11}
};

int bounds2[2] = {3, 4};

int main() {
    int koord1[] = {3};
    int i1 = Index(1, bounds1, koord1);
    int x1 = *(A1 + i1);
//     printf("A1[%d] = %d\n", i1, x1);
    printf("%d\n", i1);
    printf("%d\n", x1);

    int koord2[] = {2, 2};
    int i2 = Index(2, bounds2, koord2);
    int* T = (int*)A2;
    int x2 = *(T + i2);
//     printf("A2[%d] = %d\n", i2, x2);
    printf("%d\n", i2);
    printf("%d\n", x2);

    int koord3[] = {2, 0};
    i2 = Index(2, bounds2, koord3);
    x2 = *(T + i2);
//     printf("A2[%d] = %d\n", i2, x2);
    printf("%d\n", i2);
    printf("%d\n", x2);

    return 0;
}
