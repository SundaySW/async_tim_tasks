#pragma once

#include "async_tim_tasks/impl/task_pool.hpp"

/**
    * @brief to use:
       ASYNC_TASKS_ON_TIM place in interrupt handler for desired timer
       ASYNC_TASKS_POLL place im main loop
       place macro ASYNC_TASKS_POOL_SET_UP(
               1st @param lambda -> converting Hz to delay [](float arg)->uint32_t {... return delay;}
               2nd @param lambda -> starting []->void { StartTim(tim); }
           )
      to place a task:
            RUN_ASYNC_hz(
                1st @param expression -> expression in braces. use @var self for context {self->...}
                2nd @param float -> time in Hz for this task to repeat
            )
            @retval task N in pool(std::size_t)

            to put max frequently task RUN_ASYNC(
                1st @param expression -> expression in braces. @var self for context {self->...}
            )
            @retval task N in pool(std::size_t)

      to stop a task:
      use macro STOP_ASYNC_TASK(
            @param taskN -> which was a retval for PLACE_ASYNC_TASK_...
          )

      to resume a task:
      use macro RESUME_ASYNC_TASK(
            @param taskN -> which was a retval for PLACE_ASYNC_TASK_...
          )

      @param pool_size - default to 30 in impl/task_pool.hpp
*/

namespace async_tim_task{

#define ASYNC_TASKS_POOL_SET_UP(converter, starter) async_tim_task_impl::TaskPool::GetPool().SetUp(converter, starter)
#define ASYNC_TASKS_ON_TIM async_tim_task_impl::TaskPool::GetPool().OnTimTick();
#define ASYNC_TASKS_POLL async_tim_task_impl::TaskPool::GetPool().Poll();

#define RUN_ASYNC(expr) \
    async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT(this, [](void* context){    \
            auto self = static_cast<decltype(this)>(context);                                                       \
            expr;                                                                                                   \
        }                                                                                                           \
    ))

#define RUN_ASYNC_hz(expr, Hz)  \
    async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT(this, [](void* context){    \
            auto self = static_cast<decltype(this)>(context);                                                       \
            expr;                                                                                                   \
        }                                                                                                           \
    ), Hz)

#define RUN_ASYNC_no_context(expr, Hz)  \
        async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT([](void* context)expr), Hz)

#define RUN_ASYNC_no_context_quickest(expr)  \
        async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT([](void* context)expr))

#define REGISTER_ASYNC_TASK_suspended(expr, Hz)  \
    async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT(this, [](void* context){    \
            auto self = static_cast<decltype(this)>(context);                                                       \
            expr;                                                                                                   \
        }                                                                                                           \
    ), Hz, true)

#define RUN_ASYNC_initial_suspend_quickest(expr)  \
    async_tim_task_impl::TaskPool::GetPool().PlaceToPool(async_tim_task_impl::CallBackT(this, [](void* context){    \
            auto self = static_cast<decltype(this)>(context);                                                       \
            expr;                                                                                                   \
        }                                                                                                           \
    ), true)

#define REMOVE_ASYNC_TASK(n) async_tim_task_impl::TaskPool::GetPool().RemoveFromPool(n)
#define STOP_ASYNC_TASK(n) async_tim_task_impl::TaskPool::GetPool().StopTask(n)
#define RESUME_ASYNC_TASK(n) async_tim_task_impl::TaskPool::GetPool().ResumeTask(n)

}// namespace async_tim_task