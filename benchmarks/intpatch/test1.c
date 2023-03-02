#include <stdio.h>
#include <stdlib.h> 
int main(){
    int a = 2147483647; 
    int b = 2147483647;
    // int* ptr = malloc(b+1+a);
    printf("%d", b+3+a); // 1
    return 0;
}