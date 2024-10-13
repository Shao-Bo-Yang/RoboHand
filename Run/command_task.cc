#include "command_task.h"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "usart_rx.h"
#include <cmsis_os2.h>

namespace task
{

command_task::command_task() : _task_mapping()
{
}

void command_task::addition_init(const std::vector<command_register> &command_list)
{
    for (auto &command : command_list)
    {
        _task_mapping.insert_or_assign(command._command, command._handler);
    }
}

void command_task::operator()()
{
    for (;;)
    {
        std::string cmd = "";
        enum
        {
            WaittingToStart,
            Transmitting,
            GotEndSymbol,
            Err,
        } state = WaittingToStart;
        do
        {
            if (state == WaittingToStart)
            {
                bsd::usart_rx::use_device([&](bsd::usart_rx &device) {
                    while (device.has_data())
                    {
                        char data = device.pop_buffer();
                        if (data == '^')
                        {
                            state = Transmitting;
                            break;
                        }
                    }
                });
            }
            if (state == Transmitting)
            {
                bsd::usart_rx::use_device([&](bsd::usart_rx &device) {
                    while (device.has_data())
                    {
                        if (device.top_data() == '^')
                        {
                            state = Err;
                            return;
                        }
                        else if (device.top_data() == '$')
                        {
                            device.pop_buffer();
                            state = GotEndSymbol;
                            return;
                        }
                        else
                        {
                            cmd.push_back(device.pop_buffer());
                            if (cmd.length() > _command_max_length)
                            {
                                state = Err;
                                return;
                            }
                        }
                    }
                });
            }
            osDelay(1);
        } while (state != GotEndSymbol && state != Err);
        if (state == Err)
        {
            continue;
        }

        std::stringstream ss(cmd);
        std::string tmp;
        std::vector<std::string> args;
        while (ss >> tmp)
        {
            args.push_back(tmp);
        }
        do
        {
            if (args.size() == 0)
            {
                break;
            }
            std::reverse(args.begin() + 1, args.end());
            std::reverse(args.begin(), args.end());
            auto command = args.back();
            // command = "store_pwm_value";
            args.pop_back();
            auto handler_find = _task_mapping.find(command);
            if (handler_find == _task_mapping.end())
            {
                break;
            }
            auto &handler = handler_find->second;
            handler(args);
        } while (0);

        osDelay(1);
    }
}

} // namespace task
