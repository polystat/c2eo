long long x = 1;

int main() {
  switch(1) {break;}
  switch(1) {}
  switch(1) {
    case 1:
      printf("%lld\n", x);
  }
  switch(1) {
      default:
        printf("%lld\n", x);
  }
  return 0;
}