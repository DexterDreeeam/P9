
#include "../_Interface.hpp"

int main()
{
    s64 count = 0;
    while (!am_i_terminated() && ++count < 10)
    {
        tick::sleep(1000);
        print("not teminated yet.\n");
        yield();
    }
    return 0;
}
