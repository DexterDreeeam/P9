
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
    for (s64 i = 0; i < s64_max; ++i)
    {
        auto rA1 = ref<test_class_A>::new_instance();
        obs<test_class_A> oA1;
        {
            auto rA2 = rA1;
            oA1 = rA2;
        }
    }

    log("ok");
    log("test: %d", 1);
    print("123, %d\n", 1);

    traverse_folders_files(
        "D:/codes/P9/P9SDK/Utility/Json/",
        [](const string& s)
        {
            print("%s\n", s.data());
        },
        [](const string& s)
        {
            print("%s\n", s.data());
        });

    while (!am_i_terminated())
    {
        yield();
    }
    return 0;
}
