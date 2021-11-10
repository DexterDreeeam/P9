
#include "../Sdk/Interface.hpp"

int main()
{
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
