#pragma once

#include <functional>

#include "fwd.hpp"

namespace async_tim_task{

class AsyncTask {
public:
    AsyncTask() = default;

    explicit AsyncTask(CallBackT&& handler, DelayT delay, bool suspended = false)
        : handler_(std::move(handler))
        , interval_(delay)
    {
        inited_ = true;
        if(!suspended)
            Enable();
    }

    void TickHandle(){
        if(disabled_ || has_pending_)
            return;
        count_ += kTick_freq_;
        if(count_ >= interval_)
            has_pending_ = true;
    }

    void Poll(){
        if(has_pending_){
            handler_();
            has_pending_ = false;
            count_ = 0;
        }
    }

    void SetDelay(unsigned int delay){
        interval_ = delay;
    }

    void Enable(){
        disabled_ = false;
    }

    void Disable(){
        disabled_ = true;
    }

    bool IsInited(){
        return inited_;
    }

    void Reset(){
        inited_ = true;
    }
private:
    uint32_t count_{0};
    uint32_t interval_{0};
    uint32_t kTick_freq_ = 1;
    bool inited_ {false};
    bool disabled_ {true};
    bool has_pending_ {false};
    CallBackT handler_;
};

}// namespace async_tim_task
