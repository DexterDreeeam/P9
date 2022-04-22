#pragma once

void UT_Functional_1(u64 p1, u32 p2)
{
    print("p1: %llu, p2: %lu\n", p1, p2);
}

void UT_Functional()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    functional<u64, u32> fo = UT_Functional_1;
    fo(30, 20);
    fo(50, 40);
    fo = [](u64 p1, u32 p2)
    {
        print("p2: %lu, p1: %llu\n", p2, p1);
    };
    fo(50, 40);
}
