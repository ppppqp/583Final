#include <stdio.h>

int main() {
    int x = 2147483647;  // max value for a 32-bit signed integer
    int y = 10;
    int z = x+y;
    if(x > 0 && y > 0 && z < 0){
      printf("yo!");
    }
    return 0;
}

