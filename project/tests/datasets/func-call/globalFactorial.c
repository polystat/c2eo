int n;
int rez = 1;

void fact() {
    if(n > 1) {
    	rez = rez * n;
    	n = n - 1;
    	fact();
    }
    return;
}

int main() {
    n = 5;
    fact();
}
