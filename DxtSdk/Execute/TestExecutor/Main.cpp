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

    auto f = input_file_create("C:/dxtSDK/test.txt");
    char buf[512];
    input_file_read(f, buf, 512);
    str_pick_out_blank(buf);

    log(buf);

    auto j = JsonDeserialize(buf);

    log("%s", j->type_name());

    return 0;
}
