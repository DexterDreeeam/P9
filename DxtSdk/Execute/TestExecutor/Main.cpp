#include "../../DxtSdk/DxtSdk.h"

void myFunc(int a[])
{
    a[1] = 0;
}

int main()
{
    log("123");
    err("456");

    int b[10];
    myFunc(b);

    return 0;
}
