#pragma once

void UT_Sleep()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    tick::elapse_init();
    auto us_start = quartz::count_us();

    // 200 ms
    for (s64 i = 0; i < 10; ++i)
    {
        auto u1 = quartz::count_us();
        tick::sleep(20);
        auto udiff = quartz::count_us() - u1;
        print("%lld\n", (s64)udiff);
    }

    // 100 ms
    tick::sleep(100);

    auto total_ms = tick::elapse();
    auto us_diff = quartz::count_us() - us_start;

    assert(total_ms >= 250 && total_ms <= 500);
    assert(us_diff / 1000 >= 250 && us_diff / 1000 <= 500);
}
