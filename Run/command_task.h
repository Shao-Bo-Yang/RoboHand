#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "robo_task.h"

namespace task
{

struct command_task;

struct command_register
{
    command_register(const std::string &command,
                     const std::function<void(const std::vector<std::string> &args)> &handler)
        : _handler(handler), _command(command)
    {
    }

  private:
    std::function<void(const std::vector<std::string> &args)> _handler;
    std::string _command;
    friend command_task;
};

struct command_task : public task_generic<command_task>
{
  public:
    void addition_init(const std::vector<command_register> &command_list);

  public:
    void operator()();

  private:
    command_task();
    ~command_task() = default;

  private:
    std::unordered_map<std::string, std::function<void(const std::vector<std::string> &args)>> _task_mapping;
    static constexpr uint32_t _command_max_length = 100;
    friend task_generic;
};

} // namespace task
