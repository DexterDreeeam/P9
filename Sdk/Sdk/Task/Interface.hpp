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
 *         __Return__ = promise_object.get_return_object();
 *         if (co_await promise_object.initial_suspend())
 *         {
 *             return __Return__;
 *         }
 *
 *         try
 *         {
 *             // Coroutine Foo Body
 *
 *             // 1. co_return {expr}
 *             promise_object.return_value({expr});
 *             goto final_suspend;
 *
 *
 *             // 2. co_return
 *             promise_object.return_void();
 *             goto final_suspend;
 *
 *
 *             // 3. co_yield {expr}
 *             promise_object.yield_value({expr});
 *
 *
 *             // 4. co_await {awaiter}
 *             #IF promise_object.await_transform() ACCEPT awaiter
 *                 awaiter = promise_object.await_transform(awaiter);
 *             #ENDIF
 *             if (awaiter.await_ready() == false)
 *             {
 *                 MyCoroutineHandle = std::coroutine_handle<PromiseTy>::from_promise(promise_object);
 *                 awaiter.await_suspend(MyCoroutineHandle);
 *             }
 *             await_rst = awaiter.await_resume();
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

#define __PLACEMENT_NEW_INLINE
#define __PLACEMENT_VEC_NEW_INLINE
#include <coroutine>

template<typename PromiseTy>
using CoroTy = std::coroutine_handle<PromiseTy>;

using CoroCtxTy = std::coroutine_handle<>;

#define _Await_   co_await
#define _Return_  co_return

//
// TaskSet function contains several Task function and Action function, TaskSet function is entry point for all Task-like function
//    _Await_  Task               will raise a new thread
//    _Await_  Action             will execute function in current thread
//    _Return_ TaskSet<RetTy>
//
// Task function contains several Task function and Action function
//    _Await_  Task               will raise a new thread
//    _Await_  Action             will execute function in current thread
//    _Return_ Task<RetTy>
//
// Action function contains several Task function and Action function
//    _Await_  Task               will raise a new thread
//    _Await_  Action             will execute function in current thread
//    _Return_ Action<RetTy>
//

#include "Action.hpp"
#include "Task.hpp"
#include "TaskSet.hpp"
