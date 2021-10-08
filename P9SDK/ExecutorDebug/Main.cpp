#include "../Interface/Interface.h"

int main()
{
    auto f = input_file_create("C:/P9_Folder/test.txt");
    char buf[8000];
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    print("%d\n", len);
    str_pick_out_blank(buf);

    string str = buf;

    //log(buf);

    auto r = ref<int>::new_instance(1);

    auto j = json_deserialize(buf);

    auto cur = json_cursor(j);
    cur = cur["menu"]["popup"]["menuitem"][2]["onclick"];
    auto ele = *cur;

    print("\n%s\n", ele.data());

    // log("dexter-test %S", ele);

    // json_report(j);

    log("%s", j->type_name());

    delete j;

    return 0;
}
