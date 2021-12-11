
#include "../Sdk/Interface.hpp"

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

    while (!am_i_terminated())
    {
        tick::sleep(1000);
        print("not teminated yet.\n");
        yield();
    }
    return 0;
}
