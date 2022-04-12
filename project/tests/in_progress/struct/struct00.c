typedef struct st1 st1;
typedef struct st2 st2;

struct st2{
  int a, b, c[3];
};

struct st1{
  st2 a, b[3], c;
} obj[5];

int main() {
  obj[2].b[2].c[2] = 7;
  obj[2].b[2].c[2];
//  2[obj[2].b[2].c];
//  2[2[obj[2].b].c];
//  2[2[2[obj].b].c];
  return 0;
}