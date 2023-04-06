// Test 4
// Test overflow between multiplication of 2 integers
// (at least one of the operands is type 11).

#include <stdio.h>

int main() {
    int input;
    FILE* pFile;
    pFile = fopen("input4", "r");
    fscanf(pFile, "%d", &input);
    printf("%d", input);
    int mul10 = input*10;
    printf("%d", mul10);
    fclose(pFile);
    return 0;
}
