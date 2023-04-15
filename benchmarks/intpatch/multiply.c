#include <stdio.h>
#include <stdlib.h> 
void fn(int param1){
    return;
}
int main(){
    int input;
    FILE* pFile;
    pFile = fopen("input3", "r");
    fscanf(pFile, "%d", &input);
    printf("%d", input);
    int mult10=input*10;
    printf("%d", mult10);
    fclose(pFile);
    // int* ptr = malloc(b+1+a);
    // fn(b+3+a);
    // printf("%d", b+3+a); // 1
    return 0;
}