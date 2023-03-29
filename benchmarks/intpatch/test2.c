#include <stdio.h>

int fib(int n) {
    if (n == 1) {
        return 1;
    }
    else if (n == 2) {
        return 1;
    }
    else if (n == 3) {
        return 2;
    }
    else {
        return fib(n-1) + fib(n-2);
    }
}

int main() {
    int n = 0;
    FILE* pFile;
    pFile = fopen("input1", "r");
    fscanf(pFile, "%d", &n);
    int res = fib(n);
    fprintf(stdout, "result: %d", res);
}