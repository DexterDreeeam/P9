
#include "../_Interface.hpp"

void UT_Result();

int main()
{
    UT_Result();

    s64 count = 0;
    while (!am_i_terminated() && ++count < 10)
    {
        tick::sleep(1000);
        print("not teminated yet.\n");
        yield();
    }
    return 0;
}
