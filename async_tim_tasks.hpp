#pragma once

#include "fwd.hpp"
#include "task_pool.hpp"

namespace async_tim_task{

TaskPoolRef GetTaskPool();
#define TASK_POOL_ON_TIM()  async_tim_task::GetTaskPool().OnTimTick()
#define TASK_POOL_POLL()  async_tim_task::GetTaskPool().Poll()
#define PLACE_ASYNC_TASK(callback, delay)  async_tim_task::GetTaskPool().PlaceToPool(callback, delay)
#define REMOVE_TASK(n)  async_tim_task::GetTaskPool().RemoveFromPool(n)

 /**
     * @brief to use:
        use MACRO - TASK_POOL_ON_TIM in interrupt handler
        and MACRO - TASK_POOL_POLL im poll loop
 */

}// namespace async_tim_task

