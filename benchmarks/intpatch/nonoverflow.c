#include <stdio.h>
#include <stdlib.h> 
void fn(int param1){
    return;
}
int main(){
    char charArray[100];
    int input;
    FILE* pFile;
    pFile = fopen("input2", "r");
    fscanf(pFile, "%d", &input);
    printf("%d", input);
    for(int i = 0; i < 100; i++){
        input++;
    }
    printf("%d", input);
    fclose(pFile);
    return 0;
}