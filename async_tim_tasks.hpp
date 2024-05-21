#pragma once

#include "async_tim_tasks/impl/task_pool.hpp"

/**
    * @brief to use:
       place macro TASK_POOL_ON_TIM() in interrupt handler for desired timer
       place macro TASK_POOL_POLL() im main loop
       place macro TASK_POOL_SET_UP (
           1st @arg -> converting Hz to delay lambda [](float arg){return uint32_t delay;}
           2nd @arg -> starting lambda []{StartTim(tim);}
           )
      to place a task:
      type macro PLACE_ASYNC_TASK(
            1st @arg -> lambda [&]{} with a task
            2nd @arg -> time in Hz for this task to repeat
        ) it has a @retval with task N in pool(std::size_t)
      or to put max frequently task use PLACE_ASYNC_QUICKEST(
            @arg -> lambda [&]{} with a task
        ) it has a @retval with task N in pool(std::size_t)

      to stop a task:
      use macro STOP_TASK(
            @arg -> task N which was a retval for PLACE_ASYNC_TASK_...
          )

      to resume a task:
      use macro RESUME_TASK(
            @arg -> task N which was a retval for PLACE_ASYNC_TASK_...
          )
*/

namespace async_tim_task{

#define TASK_POOL_SET_UP(converter, starter)  async_tim_task_impl::TaskPool::GetPool().SetUp(converter, starter)

#define TASK_POOL_ON_TIM()  async_tim_task_impl::TaskPool::GetPool().OnTimTick()
#define TASK_POOL_POLL()  async_tim_task_impl::TaskPool::GetPool().Poll()

#define PLACE_ASYNC_TASK(callback, Hz)  async_tim_task_impl::TaskPool::GetPool().PlaceToPool(callback, Hz)
#define PLACE_ASYNC_QUICKEST(callback)  async_tim_task_impl::TaskPool::GetPool().PlaceToPool(callback)

#define PLACE_ASYNC_TASK_SUSPENDED(callback, Hz)  async_tim_task_impl::TaskPool::GetPool().PlaceToPool(callback, Hz, true)
#define PLACE_ASYNC_TASK_SUSPENDED_QUICKEST(callback)  async_tim_task_impl::TaskPool::GetPool().PlaceToPool(callback, true)

#define REMOVE_TASK(n)  async_tim_task_impl::TaskPool::GetPool().RemoveFromPool(n)
#define STOP_TASK(n)  async_tim_task_impl::TaskPool::GetPool().StopTask(n)
#define RESUME_TASK(n)  async_tim_task_impl::TaskPool::GetPool().ResumeTask(n)

}// namespace async_tim_task