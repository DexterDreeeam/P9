#pragma once

// Coroutine Flow
/*
 *
 *     Task<ExpectedValueType> Coroutine_Foo(Args... args)
 *     {
 *         using TaskTy = Task<ExpectedValueType>;
 *         using PromiseTy = TaskTy::promise_type;
 *
 *         promise_object = PromiseTy(args...) or PromiseTy();
 *         TaskTy __Return__ = promise_object.get_return_object();
 *
 *         if (co_await promise_object.initial_suspend())
 *         {
 *             return __Return__;
 *         }
 *
 *         try
 *         {
 *             // Coroutine Foo Body
 *
 *             // 1. co_yield {expr}
 *             promise_object.yield_value({expr});
 *
 *             // 2. co_await {awaiter}
 *             #IF promise_object.await_transform() ACCEPT awaiter
 *                 awaiter = promise_object.await_transform(awaiter);
 *             #ENDIF
 *             if (awaiter.await_ready() == false)
 *             {
 *                 MyCoroutineHandle = std::coroutine_handle<PromiseTy>::from_promise(promise_object);
 *                 awaiter.await_suspend(MyCoroutineHandle);
 *             }
 *             auto await_rst = awaiter.await_resume();
 *
 *             // 3. co_return {expr}
 *             promise_object.return_value({expr});
 *             goto final_suspend;
 *
 *             // 4. co_return
 *             promise_object.return_void();
 *             goto final_suspend;
 *
 *         }
 *         catch (...)
 *         {
 *             promise_object.unhandled_exception(std::current_exception());
 *         }
 *
 *     final_suspend:
 *         co_await promise_object.final_suspend();
 *     }
 *
 *
 */

#if defined(I_AM_LINUX_GCC)

 // >= g++-10
 // -fcoroutines
#include <coroutine>

#elif defined(I_AM_WINDOWS_MSVC)

#define __PLACEMENT_NEW_INLINE
#define __PLACEMENT_VEC_NEW_INLINE
#include <coroutine>

#else

#error "Not supported environment"

#endif

/*
 *
 * call task_async function                - raise new thread (sub_thread) to run
 *
 * co_await [not complete] task_async      - current thread return, execute remaining with sub_thread (created by one task_async function call)
 *
 * co_await [completed] task_async         - go through with current thread
 *
 *
 * call task function                      - execute in current thread until co_await keyword in it
 *
 * co_await [not complete] task            - current thread return, execute remaining with sub_thread (created by one task_async function call)
 *
 * co_await [completed] task               - go through with current thread
 *
 */


#include "_Definition.hpp"

#include "TaskAsync.hpp"
#include "Task.hpp"

#include "TaskAsync.inl"
#include "Task.inl"
