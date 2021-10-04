#include "../DxtSdk/DxtSdk.h"

void myFunc(int a[])
{
    a[1] = 0;
}

int main()
{
    int b[10];
    myFunc(b);

    auto f = input_file_create("C:/dxtSDK/test.txt");
    char buf[8000];
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    print("%d\n", len);
    str_pick_out_blank(buf);

    string str = buf;

    //log(buf);

    auto j = JsonDeserialize(buf);

    auto cur = json_cursor(j);
    cur = cur["Actors"][1]["mother"];
    auto ele = *cur;

    print("\n%s\n", ele.data());

    // log("dexter-test %S", ele);

    // json_report(j);

    log("%s", j->type_name());

    return 0;
}