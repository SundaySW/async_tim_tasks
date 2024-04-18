#pragma once

#include "fwd.hpp"
#include "task_pool.hpp"

namespace async_tim_task{

#define delay_tim_  (htim1)

TaskPoolRef GetTaskPool();

#define Hz_to_delay(v)    ((uint32_t)(SystemCoreClock / (delay_tim_.Instance->PSC) / (delay_tim_.Instance->ARR) / v))

#define TASK_POOL_ON_TIM()  async_tim_task::GetTaskPool().OnTimTick()
#define TASK_POOL_POLL()  async_tim_task::GetTaskPool().Poll()

#define PLACE_ASYNC_TASK(callback, Hz)  async_tim_task::GetTaskPool().PlaceToPool(callback, (Hz_to_delay(Hz) ? Hz_to_delay(Hz) : 1))
#define PLACE_ASYNC_TASK_SUSPENDED(callback, Hz)  async_tim_task::GetTaskPool().PlaceToPool(callback, (Hz_to_delay(Hz) ? Hz_to_delay(Hz) : 1), true)

#define REMOVE_TASK(n)  async_tim_task::GetTaskPool().RemoveFromPool(n)
#define STOP_TASK(n)  async_tim_task::GetTaskPool().StopTask(n)
#define RESUME_TASK(n)  async_tim_task::GetTaskPool().ResumeTask(n)

 /**
     * @brief to use:
        use MACRO - TASK_POOL_ON_TIM() in interrupt handler
        and MACRO - TASK_POOL_POLL() im poll loop
 */

}// namespace async_tim_task

