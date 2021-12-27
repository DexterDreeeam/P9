#pragma once

task<s64> UT_Task_func_task_1(s64 x)
{
    tick::sleep(50);
    _Return_ x * x * x;
}

task<> UT_Task_func_task_2(s64 input, s64& output)
{
    tick::sleep(50);
    output = input * input;
    _Return_;
}

task_set<> UT_TaskSet_1()
{
    s64 rst_2;

    auto task_1 = UT_Task_func_task_1(2).execute_async();
    auto task_2 = UT_Task_func_task_2(3, rst_2).execute_async();

    tick::sleep(30);

    auto rst_1 = _Await_ task_1;
    _Await_ task_2;

    print("%d\n", rst_1 + rst_2);
    _Return_;
}

task_set<s64> UT_TaskSet_2()
{
    s64 rst_2;

    auto task_1 = UT_Task_func_task_1(2).execute_async();
    auto task_2 = UT_Task_func_task_2(3, rst_2).execute_async();

    tick::sleep(30);

    auto rst_1 = _Await_ task_1;
    _Await_ task_2;

    _Return_ rst_1 + rst_2;
}

void UT_Task()
{
    auto task_set_1 = UT_TaskSet_1();
    task_set_1.await_complete();

    auto task_set_2 = UT_TaskSet_2();
    auto rst = task_set_2.await_complete();

    print("UT_Task done, result: %d\n", rst);
}
