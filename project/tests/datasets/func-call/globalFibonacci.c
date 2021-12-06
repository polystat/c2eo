int n = 4;
int tmp = 1;
int curr = 1;
int prev = 0;

void fibNum() {
    if(n > 0) {
        tmp = curr;
        curr = curr + prev;
        prev = tmp;
        n = n - 1;
        fibNum();
    }
}

int main(){
    n = 4;
    fibNum();
    curr;
}

