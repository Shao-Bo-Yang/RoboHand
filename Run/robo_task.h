#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <type_traits>
#include <utility>

#include <cmsis_os2.h>

namespace task
{

template <typename Task> struct task_generic;

template <typename Task> struct task_impl
{
    osThreadId_t task_id = nullptr;
    void init(task_generic<Task> &task, const char *name, osPriority_t priority, uint32_t stack_size = 1024 * 4)
    {
        osThreadAttr_t attr{};
        std::memset(&attr, 0, sizeof(attr));
        attr.name = name;
        attr.priority = priority;
        attr.stack_size = stack_size;
        task_id = osThreadNew((void (*)(void *))_run_helper, &task, &attr);
    }

    ~task_impl()
    {
        if (task_id && osThreadGetState(task_id) != osThreadTerminated)
        {
            osThreadTerminate(task_id);
        }
        task_id = nullptr;
    }

  private:
    static void _run_helper(task_generic<Task> *task)
    {
        task->_task_run();
    }
};

struct addition_init_trait
{
    template <typename Task> static auto has_addition_init(...) -> std::false_type;

    template <typename Task, typename = decltype(&Task::addition_init)>
    static auto has_addition_init(void *) -> std::true_type;
};

template <typename Task> struct task_generic
{
    static Task &instance()
    {
        static std::once_flag once;
        static Task instance{};
        std::call_once(once, [&]() { instance._task = std::make_unique<task_impl<Task>>(); });
        return instance;
    }

    template <typename... AdditionParameters>
    void init(const char *name, osPriority_t priority, AdditionParameters &&...parameter)
    {
        Task &task = static_cast<Task &>(*this);
        this->_task->init(task, name, priority);
        if constexpr (decltype(addition_init_trait::has_addition_init<Task>(nullptr))::value)
        {
            task.addition_init(std::forward<AdditionParameters &&>(parameter)...);
        }
    }
    osThreadId_t get_task_id()
    {
        return this->_task->task_id;
    }

  private:
    void _task_run()
    {
        Task &task = static_cast<Task &>(*this);
        task();
    }

  private:
    std::unique_ptr<task_impl<Task>> _task = nullptr;
    friend task_impl<Task>;
};

} // namespace task
