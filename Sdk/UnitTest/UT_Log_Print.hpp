#pragma once

void UT_Log_Print()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    log("ok");
    log("test: %d", 1);
    print("123, %d\n", 1);
}
