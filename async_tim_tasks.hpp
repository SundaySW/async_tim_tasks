#pragma once

#include "async_tim_tasks/impl/task_pool.hpp"

/**
    * @brief to use:
       place macro ASYNC_TASKS_ON_TIM() in interrupt handler for desired timer
       place macro ASYNC_TASKS_POLL() im main loop
       place macro TASK_POOL_SET_UP (
               1st @param lambda -> converting Hz to delay [](float arg){... return uint32_t delay;}
               2nd @param lambda -> starting []{ StartTim(tim); }
           )
      to place a task:
          type macro PLACE_ASYNC_TASK(
                1st @param lambda -> [](void* self_ptr_as_context){static_cast<SelfT*>(context)->...}
                2nd @param float -> time in Hz for this task to repeat
            )
            @retval task N in pool(std::size_t)

          to put max frequently task use PLACE_ASYNC_QUICKEST(
                @param lambda -> [](void* self_ptr_as_context){static_cast<SelfT*>(context)->...}
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

      @param pool_size - default to 10 in impl/task_pool.hpp
*/

namespace async_tim_task{

#define TASK_POOL_SET_UP(converter, starter)  async_tim_task_impl::TaskPool::GetPool().SetUp(converter, starter)

#define ASYNC_TASKS_ON_TIM()  async_tim_task_impl::TaskPool::GetPool().OnTimTick()
#define ASYNC_TASKS_POLL()  async_tim_task_impl::TaskPool::GetPool().Poll()

#define PLACE_ASYNC_TASK(lambda, Hz)  \
                    async_tim_task_impl::TaskPool::GetPool().PlaceToPool({this, lambda}, Hz)

#define PLACE_ASYNC_TASK_static(lambda, Hz)  \
                    async_tim_task_impl::TaskPool::GetPool().PlaceToPool({lambda}, Hz)

#define PLACE_ASYNC_QUICKEST(lambda) \
                    async_tim_task_impl::TaskPool::GetPool().PlaceToPool({this, lambda})

#define PLACE_ASYNC_SUSPENDED(lambda, Hz)  \
                    async_tim_task_impl::TaskPool::GetPool().PlaceToPool({this, lambda}, Hz, true)

#define PLACE_ASYNC_SUSPENDED_QUICKEST(lambda)  \
                    async_tim_task_impl::TaskPool::GetPool().PlaceToPool({this, lambda}, true)

#define REMOVE_ASYNC_TASK(n)  async_tim_task_impl::TaskPool::GetPool().RemoveFromPool(n)
#define STOP_ASYNC_TASK(n)  async_tim_task_impl::TaskPool::GetPool().StopTask(n)
#define RESUME_ASYNC_TASK(n)  async_tim_task_impl::TaskPool::GetPool().ResumeTask(n)

}// namespace async_tim_task