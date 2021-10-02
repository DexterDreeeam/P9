#pragma once

typedef  hndl  timr;

_INLINE_ timr  timr_create(Callback_Func* fn);
_INLINE_ void  timr_trigger(timr tm, void* param, u64 ms);
_INLINE_ void  timr_destroy(timr tm);

struct timr_cb_execute_desc
{
    timr_cb_execute_desc(Callback_Func* f, void* p) : fn(f), param(p), my_handle(0) {}
    Callback_Func* const fn;
    void* param;
    volatile thrd my_handle;
};

static void timr_execute(void* p)
{
    auto* execute_desc = reinterpret_cast<timr_cb_execute_desc*>(p);
    (execute_desc->fn)(execute_desc->param);
    while (!execute_desc->my_handle)
    {
        ;
    }
    thrd_destroy(execute_desc->my_handle);
    delete execute_desc;
}

struct timr_cb_desc
{
    timr_cb_desc(Callback_Func* f) : fn(f), oplock(spin_create()), running_thread(0), running(false), param(nullptr), delay(0) {}
    ~timr_cb_desc() { spin_destroy(oplock); }
    Callback_Func* const fn;
    const spin oplock;
    thrd running_thread;
    boole running;
    void* param;
    u64 delay;
};

static void timr_cb(void* p)
{
    auto* desc = reinterpret_cast<volatile timr_cb_desc*>(p);
    auto* func = desc->fn;
    void* param = desc->param;
    spin sp = desc->oplock;
    u64 delay = desc->delay;
    desc->running = boole::True;
    spin_put(sp);

    tick_sleep(delay);

    spin_wait_get(sp);
    auto* execute_desc = new timr_cb_execute_desc(func, param);
    execute_desc->my_handle = thrd_create(timr_execute, execute_desc);
    desc->running = boole::False;
    spin_put(sp);
}

_INLINE_ timr timr_create(Callback_Func* fn)
{
    assert(fn);
    auto* desc = new timr_cb_desc(fn);
    return reinterpret_cast<timr>(desc);
}

_INLINE_ void timr_trigger(timr tm, void* param, u64 ms)
{
    assert(tm);
    auto* desc = reinterpret_cast<timr_cb_desc*>(tm);
    spin_wait_get(desc->oplock);
    if (desc->running)
    {
        desc->running = false;
        thrd_enforce_exit(desc->running_thread);
    }
    if (desc->running_thread)
    {
        thrd td = desc->running_thread;
        desc->running_thread = 0;
        thrd_destroy(td);
    }
    memory_barrier();
    desc->param = param;
    desc->delay = ms;
    desc->running_thread = thrd_create(timr_cb, desc);
}

_INLINE_ void timr_destroy(timr tm)
{
    assert(tm);
    auto* desc = reinterpret_cast<timr_cb_desc*>(tm);
    spin_wait_get(desc->oplock);
    if (desc->running)
    {
        desc->running = false;
        thrd_enforce_exit(desc->running_thread);
    }
    if (desc->running_thread)
    {
        thrd td = desc->running_thread;
        desc->running_thread = 0;
        thrd_destroy(td);
    }
    spin_put(desc->oplock);
    delete reinterpret_cast<timr_cb_desc*>(tm);
}
