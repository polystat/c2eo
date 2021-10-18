//clang -Xclang -ast-dump -fsyntax-only main.c
int f(int n) {
    if (n == 0)
    	return 1;    
    return f(n-1) * n;
}
