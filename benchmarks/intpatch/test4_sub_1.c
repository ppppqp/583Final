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
    pFile = fopen("neg_max", "r");
    fscanf(pFile, "%d", &input);
    printf("%d", input);
    int plus10=input-10;
    printf("%d", plus10);
    fclose(pFile);
    return 0;
}