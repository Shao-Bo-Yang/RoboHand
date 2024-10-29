#include "command_tasks.h"

#include <cstdint>
#include <sstream>

#include "flash.h"
#include "log.hh"
#include "pwm.h"
#include "pwm_control_task.h"
#include "switch.h"

namespace command_tasks
{

void switch_on(const std::vector<std::string> &args)
{
    auto status_str = args.front();
    int status = 0;
    std::stringstream ss(status_str);
    ss >> status;

    bsd::switch_on::use_device([status](bsd::switch_on &device) {
        bool on = false;
        if (status > 0)
        {
            device.on();
            on = true;
        }
        else
        {

            device.off();
            on = false;
        }

        base::log("switch: ", on ? "on" : "off");
    });
}

void update_pwm_value(const std::vector<std::string> &args)
{
    double duties[8];
    bsd::flash::use_device([&duties](bsd::flash &device) {
        device.read_flash(reinterpret_cast<uint32_t *>(duties), sizeof(duties) / sizeof(uint32_t));
    });
    bsd::pwm::use_device([&duties](bsd::pwm &device) {
        device.set_duty<bsd::pwm_channel::Ch1>(duties[0]);
        device.set_duty<bsd::pwm_channel::Ch2>(duties[1]);
        device.set_duty<bsd::pwm_channel::Ch3>(duties[2]);
        device.set_duty<bsd::pwm_channel::Ch4>(duties[3]);
        device.set_duty<bsd::pwm_channel::Ch5>(duties[4]);
        device.set_duty<bsd::pwm_channel::Ch6>(duties[5]);
        device.set_duty<bsd::pwm_channel::Ch7>(duties[6]);
        device.set_duty<bsd::pwm_channel::Ch8>(duties[7]);
    });

#define DUTY(i) (int(100 * duties[i]))
    base::log("Set Duty{ ch1: ", DUTY(0), ", ch2: ", DUTY(1), ", ch3: ", DUTY(2), ", ch4: ", DUTY(3),
              ", ch5: ", DUTY(4), ", ch6: ", DUTY(5), ", ch7: ", DUTY(6), ", ch8: ", DUTY(7), " }");
#undef DUTY
}

void store_pwm_value(const std::vector<std::string> &args)
{
    double duties[8];
    bsd::pwm::use_device([&duties](bsd::pwm &device) {
        duties[0] = device.get_duty<bsd::pwm_channel::Ch1>();
        duties[1] = device.get_duty<bsd::pwm_channel::Ch2>();
        duties[2] = device.get_duty<bsd::pwm_channel::Ch3>();
        duties[3] = device.get_duty<bsd::pwm_channel::Ch4>();
        duties[4] = device.get_duty<bsd::pwm_channel::Ch5>();
        duties[5] = device.get_duty<bsd::pwm_channel::Ch6>();
        duties[6] = device.get_duty<bsd::pwm_channel::Ch7>();
        duties[7] = device.get_duty<bsd::pwm_channel::Ch8>();
    });
    bsd::flash::use_device([&duties](auto &device) {
        device.write_flash(reinterpret_cast<uint32_t *>(duties), sizeof(duties) / sizeof(uint32_t));
    });

#define DUTY(i) (int(100 * duties[i]))
    base::log("Store Duty{ ch1: ", DUTY(0), ", ch2: ", DUTY(1), ", ch3: ", DUTY(2), ", ch4: ", DUTY(3),
              ", ch5: ", DUTY(4), ", ch6: ", DUTY(5), ", ch7: ", DUTY(6), ", ch8: ", DUTY(7), " }");
#undef DUTY
}

void _set_angle_task(const std::vector<std::string> &args, std::function<void(float duty)> func, bsd::pwm_channel ch)
{
    using bsd::pwm;
    if (args.size() < 1)
    {
        base::log("<set_pwm_duty_", (uint32_t)ch,
                  "> command needs at least one parameter which is a "
                  "float between 0.0 and 1.0 number");
        return;
    }
    auto angle_str = args.front();
    double angle = 0;
    std::stringstream ss(angle_str);
    ss >> angle;
    angle = std::max(std::min(angle, 170.0), -170.0);
    angle += 180;
    auto ratio = angle / 360;
    auto duty = ratio * (0.125 - 0.025) + 0.025;
    func(duty);
    base::log("Set Duty{ ch: ", (uint32_t)ch, ", angle: ", (int)angle, ", duty: ", (uint32_t)(100 * duty), " / 100",
              "}");
}

void _set_pwm_duty_task(const std::vector<std::string> &args, std::function<void(float duty)> func, bsd::pwm_channel ch)
{
    using bsd::pwm;
    if (args.size() < 1)
    {
        base::log("<set_pwm_duty_", (uint32_t)ch,
                  "> command needs at least one parameter which is a "
                  "float between 0.0 and 1.0 number");
        return;
    }
    auto duty_str = args.front();
    double duty = 0;
    std::stringstream ss(duty_str);
    ss >> duty;
    func(duty);
    // pwm::use_device([&duty, ch](pwm &device) { device.set_duty(duty, ch); });
    base::log("Set Duty{ ch: ", (uint32_t)ch, ", duty: ", (uint32_t)(100 * duty), " / 100", "}");
}

} // namespace command_tasks
