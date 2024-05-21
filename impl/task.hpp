#pragma once

#include <functional>

namespace async_tim_task_impl{

#define kTick_freq_ (1)

class AsyncTask {
public:
    using CallBackT = std::function<void()>;
    AsyncTask() = default;

    explicit AsyncTask(CallBackT&& handler, std::size_t delay, bool suspended = false)
        : handler_(std::move(handler))
        , interval_(delay)
        , inited_(true)
    {
        if(!suspended)
            Enable();
    }

    [[gnu::always_inline]] void TickHandle(){
        count_ += kTick_freq_;
    }

    [[gnu::always_inline]] void Poll(){
        if(!disabled_ && count_ >= interval_){
            handler_();
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

    [[nodiscard]] bool IsInited() const{
        return inited_;
    }

    void Reset(){
        disabled_ = true;
        inited_ = true;
    }
private:
    std::size_t count_{0};
    std::size_t interval_{0};
    bool inited_ {false};
    bool disabled_ {true};
    CallBackT handler_;
};

}// namespace async_tim_task
