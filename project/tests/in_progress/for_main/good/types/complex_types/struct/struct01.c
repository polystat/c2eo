//clang -Xclang -ast-dump -fsyntax-only main.cpp
struct st{
  int a;
};

int main() {
  struct st obj1;
  return 0;
}
