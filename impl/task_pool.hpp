#pragma once

#include <optional>

#include "async_tim_tasks/impl/task.hpp"
#include "async_tim_tasks/async_tim_tasks.hpp"

namespace async_tim_task_impl{

constexpr std::size_t k_pool_size = 20;

struct TaskPool{
    using ConverterFt = uint32_t(*)(float);
    ~TaskPool() = default;
    TaskPool(TaskPool&) = delete;
    TaskPool(TaskPool&&) = delete;
    TaskPool& operator = (TaskPool &) = delete;
    TaskPool& operator = (TaskPool const &) = delete;

    static auto& GetPool(){
        static TaskPool task_pool{};
        return task_pool;
    }

    void SetUp(ConverterFt converter, const std::function<void()>& starter){
        converter_ = converter;
        starter();
    }

    int PlaceToPool(CallBackT&& cb, float Hz = UINT32_MAX, bool suspended = false){
        assert(converter_ != nullptr);
        int idx = -1;
        for(std::size_t i = 0; i < k_pool_size; i++){
            if(!pool_[i].IsInited()){
                pool_[i] = AsyncTask{std::forward<CallBackT>(cb), converter_(Hz), suspended};
                idx = i;
                current_pool_size_++;
                break;
            }
        }
        return idx;
    }

    bool RemoveFromPool(unsigned short idx){
        if(idx >= k_pool_size)
            return false;
        pool_[idx].Reset();
        current_pool_size_--;
        return true;
    }

    bool StopTask(unsigned short idx){
        if(idx >= k_pool_size)
            return false;
        pool_[idx].Disable();
        return true;
    }

    bool ResumeTask(unsigned short idx){
        if(idx >= k_pool_size)
            return false;
        pool_[idx].Enable();
        return true;
    }

    [[gnu::always_inline]] void OnTimTick(){
        for(std::size_t i = 0; i < current_pool_size_; i++)
            pool_[i].TickHandle();
    }

    [[gnu::always_inline]] void Poll(){
        for(std::size_t i = 0; i < current_pool_size_; i++)
            pool_[i].Poll();
    }
private:
    std::size_t current_pool_size_ {0};
    TaskPool() = default;
    std::array<AsyncTask, k_pool_size> pool_;
    ConverterFt converter_;
};

}// namespace async_tim_task