#pragma once

typedef  hndl  thrd;
typedef  void  Callback_Func(void* param);

_INLINE_ void  yield();
_INLINE_ void  memory_barrier(void);
_INLINE_ thrd  thrd_create(Callback_Func* fn, void* param);
_INLINE_ u64   thrd_myid();
_INLINE_ void  thrd_enforce_exit(thrd x);
_INLINE_ void  thrd_wait_exit(thrd x);
_INLINE_ void  thrd_destroy(thrd x);

_INLINE_ void yield()
{
    pthread_yield();
}

_INLINE_ void memory_barrier(void)
{
    // todo
}

struct _thrd
{
    pthread_t _thrd_id;
};

struct thrd_cb_input
{
    thrd_cb_input(Callback_Func* f, void* p) : fn(f), param(p) {}
    Callback_Func* fn;
    void* param;
};

static void* thrd_cb_entrance(void *arg)
{
    auto* input = (thrd_cb_input*)arg;
    (input->fn)(input->param);
    delete input;
    return nullptr;
}

_INLINE_ thrd thrd_create(Callback_Func* fn, void* param)
{
    assert(fn);
    auto* input = new thrd_cb_input(fn, param);
    pthread_t pt;
    pthread_create(&pt, nullptr, thrd_cb_entrance, input);
    _thrd* td = new _thrd();
    td->_thrd_id = pt;
    return (thrd)td;
}

_INLINE_ u64 thrd_myid()
{
    return (u64)pthread_self();
}

_INLINE_ void thrd_enforce_exit(thrd x)
{
    _thrd* td = (_thrd*)x;
    assert(td && td->_thrd_id);
    pthread_kill(td->_thrd_id, SIGKILL);
}

_INLINE_ void thrd_wait_exit(thrd x)
{
    _thrd* td = (_thrd*)x;
    assert(td && td->_thrd_id);
    pthread_join(td->_thrd_id, nullptr);
}

_INLINE_ void thrd_destroy(thrd x)
{
    _thrd* td = (_thrd*)x;
    assert(td && td->_thrd_id);
    delete td;
}
