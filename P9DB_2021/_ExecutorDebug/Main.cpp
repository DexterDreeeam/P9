#include "../Interface/Interface.h"

void test_db_query(P9::Platform::platform* plt, const char* path)
{
    char buf[8000];
    auto f = input_file_create(path);
    s64 len = input_file_read(f, buf, 8000);
    buf[len] = 0;
    input_file_destroy(f);
    string s1(buf);
    auto op1 = plt->parse_operation_message(s1);
    string rst = plt->handle_operation(op1);

    print("result: %s.\n", rst.data());
    log("result: %s.", rst.data());
}

void entry_point()
{
    tick_start();

    auto p = new P9::Platform::platform();
    p->load("C:/P9_Folder/");

    test_db_query(p, "C:/P9_Folder/upsert-1.txt");
    test_db_query(p, "C:/P9_Folder/upsert-2.txt");
    test_db_query(p, "C:/P9_Folder/upsert-3.txt");
    test_db_query(p, "C:/P9_Folder/retrieve-1.txt");
}

int main()
{
    try
    {
        entry_point();
    }
    catch (...)
    {
        err("exception happens...");
    }
    return 0;
}
