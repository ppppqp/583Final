#include <stdio.h>
#include <stdlib.h> 
void fn(int param1){
    return;
}
int main(){
    int a = 2147483647; 
    int b = 2147483647;
    int input;
    FILE* pFile;
    pFile = fopen("input1", "r");
    fscanf(pFile, "%d", &input);
    printf("%d", input);
    int plus10=input+10;
    printf("%d", plus10);
    fclose(pFile);
    // int* ptr = malloc(b+1+a);
    // fn(b+3+a);
    // printf("%d", b+3+a); // 1
    return 0;
}