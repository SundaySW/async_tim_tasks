#pragma once

#include <optional>

#include "task.hpp"
#include "fwd.hpp"

namespace async_tim_task{

namespace {
    const std::size_t pool_size = 20;
}

struct TaskPool{
    ~TaskPool() = default;
    TaskPool(TaskPool&) = delete;
    TaskPool(TaskPool&&) = delete;
    TaskPool& operator = (TaskPool &) = delete;
    TaskPool& operator = (TaskPool const &) = delete;

    static TaskPoolRef GetPool(){
        static TaskPool task_pool{};
        return task_pool;
    }

    int PlaceToPool(CallBackT&& f, DelayT d, bool suspended = false){
        int idx = -1;
        for(std::size_t i = 0; i < pool_size; i++){
            if(!pool_[i].IsInited()){
                pool_[i] = AsyncTask{std::forward<CallBackT>(f), d, suspended};
                idx = i;
                break;
            }
        }
        return idx;
    }

    bool RemoveFromPool(unsigned short idx){
        if(idx >= pool_size)
            return false;
        pool_[idx].Reset();
        return true;
    }

    bool StopTask(unsigned short idx){
        if(idx >= pool_size)
            return false;
        pool_[idx].Disable();
        return true;
    }

    bool ResumeTask(unsigned short idx){
        if(idx >= pool_size)
            return false;
        pool_[idx].Enable();
        return true;
    }

    void OnTimTick(){
        for(auto& task : pool_)
            task.TickHandle();
    }

    void Poll(){
        for(auto& task : pool_)
            task.Poll();
    }
private:
    TaskPool() = default;
    std::array<AsyncTask, pool_size> pool_;
};

}// namespace async_tim_task