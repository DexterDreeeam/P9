
#include "../_Interface.hpp"
#include "../_InterfaceExt.hpp"

#include "UT_Log_Print.hpp"
#include "UT_Traverse_Directory.hpp"
#include "UT_Functional.hpp"
#include "UT_Task.hpp"
#include "UT_Object.hpp"

int main()
{
    UT_Log_Print();
    UT_Traverse_Directory();
    UT_Functional();
    UT_Task();
    UT_Object();

    s64 count = 0;
    while (!am_i_terminated() && ++count < 10)
    {
        tick::sleep(1000);
        print("not teminated yet.\n");
        yield();
    }
    return 0;
}
