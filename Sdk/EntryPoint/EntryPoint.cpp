
#include "../Interface.hpp"
#include "../Interface_Task.hpp"

#include "UT_Task.hpp"

class test_class_A
{
public:
    test_class_A()
    {
        print("test_class_A +\n");
    }

    ~test_class_A()
    {
        print("test_class_A -\n");
    }

    s64 i;
};

void functional_1(u64 p1, u32 p2)
{
    print("p1: %llu, p2: %lu\n", p1, p2);
}

int main()
{
    log("ok");
    log("test: %d", 1);
    print("123, %d\n", 1);

    // traverse_folders_files(
    //     "D:/codes/P9/P9SDK/Utility/Json/",
    //     [](const string& s)
    //     {
    //         print("%s\n", s.data());
    //     },
    //     [](const string& s)
    //     {
    //         print("%s\n", s.data());
    //     });

    quartz::count_ms();

    functional<u64, u32> fo = functional_1;
    fo(30, 20);
    fo(50, 40);
    fo = [](u64 p1, u32 p2)
    {
        print("p2: %lu, p1: %llu\n", p2, p1);
    };
    fo(50, 40);

    UT_Task();

    while (!am_i_terminated())
    {
        tick::sleep(1000);
        print("not teminated yet.\n");
        yield();
    }
    return 0;
}
