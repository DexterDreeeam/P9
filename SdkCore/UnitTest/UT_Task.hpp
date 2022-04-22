#pragma once

void UT_Task_Print_Space(s64 layer)
{
    for (s64 i = 0; i < layer; ++i)
    {
        print("   ");
    }
}

task_async<s64> UT_Task_Async_100ms_ret(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(100);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return x * x;
}

task_async<> UT_Task_Async_100ms(s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(100);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return;
}

task_async<s64> UT_Task_Async_200ms_ret(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(200);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return x * x;
}

task_async<> UT_Task_Async_200ms(s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(200);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return;
}

task<s64> UT_Task_100ms_ret(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(100);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return x * x * x;
}

task<> UT_Task_100ms(s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(100);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return;
}

task<s64> UT_Task_200ms_ret(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(200);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return x * x * x;
}

task<> UT_Task_200ms(s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    tick::sleep(200);
    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return;
}

task_async<s64> UT_Task_Async_Cubic_Plus_Square(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    auto ta100r = UT_Task_Async_100ms_ret(x, layer + 1);
    auto ta100 = UT_Task_Async_100ms(layer + 1);

    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    auto ta200r = UT_Task_Async_200ms_ret(x, layer + 1);
    auto ta200 = UT_Task_Async_200ms(layer + 1);

    UT_Task_Print_Space(layer);
    print("%s #3 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    auto r1 = co_await ta200r;
    auto r2 = co_await ta100r;

    UT_Task_Print_Space(layer);
    print("%s #4 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    assert(r1 == r2);

    auto t100r = UT_Task_100ms_ret(x, layer + 1);
    auto t100 = UT_Task_100ms(layer + 1);
    UT_Task_Print_Space(layer);
    print("%s #5 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    auto t200r = UT_Task_200ms_ret(x, layer + 1);
    auto t200 = UT_Task_200ms(layer + 1);
    UT_Task_Print_Space(layer);
    print("%s #6 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    auto r3 = co_await t200r;
    auto r4 = co_await t100r;
    UT_Task_Print_Space(layer);
    print("%s #7 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    assert(r3 == r4);
    co_return r1 + r3;
}

task<s64> UT_Task_Cubic_Plus_Square_Plus_X(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    auto cps = co_await UT_Task_Async_Cubic_Plus_Square(x, layer + 1);

    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    co_return cps + x;
}

task<s64> UT_Task_Cubic_Plus_Square_Plus_X_Plus_X(s64 x, s64 layer)
{
    UT_Task_Print_Space(layer);
    print("%s #1 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    auto cpspx1 = co_await UT_Task_Cubic_Plus_Square_Plus_X(x, layer + 1);

    UT_Task_Print_Space(layer);
    print("%s #2 run in thread: %llu\n", __FUNCTION__, current_thread_id());
    auto cpspx2 = UT_Task_Cubic_Plus_Square_Plus_X(x, layer + 1).wait_result();

    UT_Task_Print_Space(layer);
    print("%s #3 run in thread: %llu\n", __FUNCTION__, current_thread_id());

    assert(cpspx1 == cpspx2);
    co_return cpspx1 + x;
}

void UT_Task()
{
    const char* func_name = __FUNCTION__;
    print("\n>>>>>>>>> [%s] Start >>>>>>>>>\n\n", func_name);
    escape_function ef = [=]()
    {
        print("\n<<<<<<<<< [%s] End <<<<<<<<<\n\n", func_name);
    };

    print("UT_Task run in thread: %llu\n", current_thread_id());

    auto rst = UT_Task_Cubic_Plus_Square_Plus_X_Plus_X(2, 1).wait_result();

    print("UT_Task done in thread: %llu, result: %d\n", current_thread_id(), rst);
}
