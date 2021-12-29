#pragma once

task<s64> UT_Task_sub_task(s64 x)
{
    print("action_task run in thread: %llu\n", current_thread_id());
    _Return_ x * x;
}

action<> UT_Task_action_1(s64 x, s64& output)
{
    print("Action 1 run in thread: %llu\n", current_thread_id());
    output = _Await_ UT_Task_sub_task(x).execute_async();
    _Return_;
}

action<s64> UT_Task_action_2(s64 x)
{
    print("Action 2 run in thread: %llu\n", current_thread_id());
    _Return_ _Await_ UT_Task_sub_task(x).execute_async();
}

task<s64> UT_Task_func_task_1(s64 x)
{
    print("Task 1 run in thread: %llu\n", current_thread_id());
    tick::sleep(20);
    s64 x2_1;
    s64 x2_2;
    auto act_1 = UT_Task_action_1(x, x2_1);
    x2_2 = _Await_ UT_Task_action_2(x);
    _Await_ act_1;

    assert(x2_1 == x2_2);
    _Return_ x * x2_1;
}

task<> UT_Task_func_task_2(s64 input, s64& output)
{
    print("Task 2 run in thread: %llu\n", current_thread_id());
    tick::sleep(40);
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
    print("Task Set 2 P1 run in thread: %llu\n", current_thread_id());
    auto task_1 = UT_Task_func_task_1(2).execute_async();
    auto task_2 = UT_Task_func_task_2(3, rst_2).execute_async();
    tick::sleep(30);

    auto rst_1 = _Await_ task_1;
    print("Task Set 2 P2 run in thread: %llu\n", current_thread_id());

    _Await_ task_2;
    print("Task Set 2 P3 run in thread: %llu\n", current_thread_id());

    _Return_ rst_1 + rst_2;
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

    auto task_set_1 = UT_TaskSet_1();
    task_set_1.await_complete();

    auto task_set_2 = UT_TaskSet_2();
    auto rst = task_set_2.await_complete();

    print("UT_Task done, result: %d\n", rst);
}
