#pragma once

#include "functional"
#include "cstdint"

namespace async_tim_task{

using CallBackT = std::function<void()>;
//using CallBackT = fu2::function_base<true, false, fu2::capacity_fixed<call_back_capacity>,
//            false, false, void()>;
using DelayT = std::size_t;

class TaskPool;

using TaskPoolRef = TaskPool&;

}// namespace async_tim_task