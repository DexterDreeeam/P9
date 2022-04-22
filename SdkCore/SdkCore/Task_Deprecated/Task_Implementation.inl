#pragma once

_INLINE_ bool task_base::await_ready()
{
    auto& l_sync_point = *_sync_point.raw_ptr();
    s64 my_order = ++l_sync_point;
    assert(my_order == 1 || my_order == 2);

    if (my_order == 1)
    {
        // _Await_ this Task, but it doesn't complete
        return false;
    }
    else
    {
        return true;
    }
}

_INLINE_ bool task_base::await_suspend(CoroCtxTy coro)
{
    *_parent_coro_addr.raw_ptr() = coro.address();

    auto& l_sync_point = *_sync_point.raw_ptr();
    ++l_sync_point;

    return true;
}

_INLINE_ task_base::task_base(CoroCtxTy coro) :
    _sync_point(ref<atom<s64>>::new_instance(0)),
    _coro(coro),
    _parent_coro_addr(ref<void*>::new_instance(nullptr)),
    _p_action_sync_event(nullptr)
{
}

_INLINE_ task_base::task_base(const task_base& rhs) :
    _sync_point(rhs._sync_point),
    _coro(rhs._coro),
    _parent_coro_addr(rhs._parent_coro_addr),
    _p_action_sync_event(rhs._p_action_sync_event)
{
}

_INLINE_ task_base& task_base::operator =(const task_base& rhs)
{
    _sync_point = rhs._sync_point;
    _coro = rhs._coro;
    _parent_coro_addr = rhs._parent_coro_addr;
    _p_action_sync_event = rhs._p_action_sync_event;
    return *this;
}

_INLINE_ void task_base::execute_async()
{
    thread t;
    t.init(task_base::execute_UnitTest);

    auto* p = new callback_context();
    p->sync_point = _sync_point;
    p->coro = _coro;
    p->parent_coro_addr = _parent_coro_addr;
    p->pp_action_sync_event = &_p_action_sync_event;

    t.start(p);
    t.uninit();
}

_INLINE_ void task_base::execute_UnitTest(void* p)
{
    callback_context* p_ctx = pointer_convert(p, 0, callback_context*);

    // store context content
    auto sync_point = p_ctx->sync_point;
    auto task_coro = p_ctx->coro;
    auto parent_coro_addr = p_ctx->parent_coro_addr;
    auto pp_action_sync_event = p_ctx->pp_action_sync_event;

    delete p_ctx;

    // execute Task function body
    task_coro.resume();

    auto& l_sync_point = *sync_point.raw_ptr();
    s64 my_order = ++l_sync_point;
    assert(my_order == 1 || my_order == 2 || my_order == 3);

    if (my_order == 1)
    {
        // no _Await_ operator waiting this Task now
        return;
    }

    if (*pp_action_sync_event)
    {
        // action call task
        // action _Await_ done
        // notify action thread
        (*pp_action_sync_event)->set();
    }

    while (my_order == 2)
    {
        yield();
        my_order = l_sync_point.get();
    }

    assert(my_order == 3);
    // resume Caller's execute body
    auto parent_coro = CoroCtxTy::from_address(*parent_coro_addr.raw_ptr());
    parent_coro.resume();
}
