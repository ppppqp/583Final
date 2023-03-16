#include <stdio.h>
#include <stdlib.h> 
void fn(int param1){
    return;
}
int main(){
    int a = 2147483647; 
    int b = 2147483647;
    char input[20];
    scanf("%s", input);
    // int* ptr = malloc(b+1+a);
    fn(b+3+a);
    printf("%d", b+3+a); // 1
    return 0;
}