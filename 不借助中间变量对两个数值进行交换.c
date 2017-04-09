#include <stdio.h>

int main(int argc,char *argv[]){
    int a = 10,b = 4;
    printf("%d\n%d\n",a,b);
    a = a+b;
    b = a-b;
    a = a-b;
    printf("%d\n%d\n",a,b);
    return 0;
}
