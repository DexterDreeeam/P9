#include "../Interface/Interface.hpp"

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

    print("result: %s.\n\n", rst.data());
    log("result: %s.", rst.data());
}

void entry_point()
{
    tick_start();

    auto p = new P9::Platform::platform();
    p->load(P9_FOLDER "/p1/");

    test_db_query(p, "../.tests/01-upsert-1.json");
    test_db_query(p, "../.tests/02-upsert-2.json");
    test_db_query(p, "../.tests/03-upsert-3.json");
    test_db_query(p, "../.tests/04-retrieve-1.json");
    test_db_query(p, "../.tests/05-search-1.json");
    test_db_query(p, "../.tests/06-search-2.json");
    test_db_query(p, "../.tests/07-search-3.json");
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
