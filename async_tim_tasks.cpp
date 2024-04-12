#include "async_tim_tasks.hpp"
#include "task_pool.hpp"

namespace async_tim_task{

TaskPoolRef GetTaskPool(){
    return TaskPool::GetPool();
}

}// namespace async_tim_task