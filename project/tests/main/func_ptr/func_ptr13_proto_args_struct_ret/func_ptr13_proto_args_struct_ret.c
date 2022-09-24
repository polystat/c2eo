// #include <stdio.h>

struct rect{int x, y;} r1 = {10, 30};
struct rect r2;
struct rect *p_rect;

struct rect func_struct(struct rect r) {
  r.x++;
  r.y--;
  return r;
}
struct rect (*func_struct_ptr)(struct rect);

int main() {
  func_struct_ptr = func_struct;
  r2 = func_struct(r1);
//   printf("r2={%d, %d}\n", r2.x, r2.y);

  r1 = func_struct_ptr(r2);
//   printf("r1={%d, %d}\n", r1.x, r1.y);

  p_rect = &r1;
  r2 = func_struct(*p_rect);
//   printf("r2={%d, %d}\n", r2.x, r2.y);

  return 0;
}
