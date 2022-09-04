struct S {int a; int b;};
struct S arr[2] = {[1] = {3, 4}, [0] = {1, 2}};
int arr2[2][2] = {[1] = {3, 4}, [0] = {1, 2}};
struct S1 {struct S a; struct S b;};
struct S1 st = {.b = {3, 4}, .a = {1, 2}};
int
test()
{
  printf("%d %d %d %d\n", arr[0].a, arr[0].b, arr[1].a, arr[1].b);
  printf("%d %d %d %d\n", arr2[0][0], arr2[0][1], arr2[1][0], arr2[1][1]);
  printf("%d %d %d %d\n", st.a.a, st.a.b, st.b.a, st.b.b);
  return 0;
}

#include <stdio.h>
int main()
{
	int x = test();
	printf("%d\n", x);
	return x;
}
