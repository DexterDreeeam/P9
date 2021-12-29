#pragma once

template<typename TaskTy>
_INLINE_ void action_base::ActionPromiseBase::await_transform_internal(TaskTy& task_awaiter)
{
    _TaskNs::sync_event_wrapper action_sync_event;
    task_awaiter.set_action_sync_event(&action_sync_event);
    auto& sync_point = *task_awaiter._sync_point.raw_ptr();
    s64 my_order = ++sync_point;
    if (my_order == 1)
    {
        // block, wait task complete
        action_sync_event.wait();
    }
}

_INLINE_ _TaskNs::suspend_never<void> action_base::ActionPromiseBase::await_transform(task<void>& awaiter)
{
    await_transform_internal(awaiter);
    return _TaskNs::suspend_never<void>();
}

_INLINE_ _TaskNs::suspend_never<void> action_base::ActionPromiseBase::await_transform(task<void>&& awaiter)
{
    await_transform_internal(awaiter);
    return _TaskNs::suspend_never<void>();
}

template<typename SubTaskRetTy>
_INLINE_ _TaskNs::suspend_never<SubTaskRetTy> action_base::ActionPromiseBase::await_transform(task<SubTaskRetTy>& awaiter)
{
    await_transform_internal(awaiter);
    return _TaskNs::suspend_never<SubTaskRetTy>(awaiter.result_ref());
}

template<typename SubTaskRetTy>
_INLINE_ _TaskNs::suspend_never<SubTaskRetTy> action_base::ActionPromiseBase::await_transform(task<SubTaskRetTy>&& awaiter)
{
    await_transform_internal(awaiter);
    return _TaskNs::suspend_never<SubTaskRetTy>(awaiter.result_ref());
}
