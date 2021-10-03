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
    char buf[8000];
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    print("%d\n", len);
    str_pick_out_blank(buf);

    string str = buf;

    //log(buf);

    auto j = JsonDeserialize(buf);
    j->Iterate(
        [](JsonNs::json_base* json)
        {
            log("my json type is %s", json->type_name());
            log("my json path is %s", json->my_path().data());
        });

    json_report(j);

    log("%s", j->type_name());

    return 0;
}
