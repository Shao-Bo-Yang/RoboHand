#include "root_task.h"

#include <vector>

#include "app_time.h"
#include "command_task.h"
#include "command_tasks/command_tasks.h"
#include "err_led_task.h"
#include "flash.h"
#include "led.h"
#include "pwm.h"
#include "pwm_control_task.h"
#include "pwm_led_task.h"
#include "switch.h"
#include "task.h"
#include "usart_log_task.h"
#include "usart_rx.h"
#include "usart_tx.h"
#include "work_led_task.h"
extern "C"
{
#include <FreeRTOS.h>
#include <cmsis_os2.h>
}

void root_task_handler(void *argument)
{
    taskENTER_CRITICAL();
    using namespace bsd;
    using namespace task;
    app_time::use_device([](app_time &device) { device.init(); });
    pwm::use_device([](pwm &device) { device.init(); });
    flash::use_device([](flash &device) { device.init(); });
    led::use_device([](led &device) { device.init(); });
    usart_rx::use_device([](usart_rx &device) { device.init(); });
    usart_tx::use_device([](usart_tx &device) { device.init(); });
    switch_on::use_device([](switch_on &device) { device.init(); });

    pwm_led_task::instance().init("pwm_led_task", osPriorityLow);
    work_led_task::instance().init("work_led_task", osPriorityLow);
    err_led_task::instance().init("err_led_task", osPriorityLow);
    usart_log_task::instance().init("usart_log_task", osPriorityLow);
    pwm_control_task::instance().init("pwm_control_task", osPriorityNormal);
    command_task::instance().init("command_task", osPriorityHigh,
                                  std::vector<command_register>{
                                      {"set_pwm_duty_1", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch1>},
                                      {"set_pwm_duty_2", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch2>},
                                      {"set_pwm_duty_3", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch3>},
                                      {"set_pwm_duty_4", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch4>},
                                      {"set_pwm_duty_5", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch5>},
                                      {"set_pwm_duty_6", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch6>},
                                      {"set_pwm_duty_7", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch7>},
                                      {"set_pwm_duty_8", command_tasks::set_pwm_duty_task<bsd::pwm_channel::Ch8>},
                                      {"set_angle_1", command_tasks::set_angle_task<bsd::pwm_channel::Ch1>},
                                      {"set_angle_2", command_tasks::set_angle_task<bsd::pwm_channel::Ch2>},
                                      {"set_angle_3", command_tasks::set_angle_task<bsd::pwm_channel::Ch3>},
                                      {"set_angle_4", command_tasks::set_angle_task<bsd::pwm_channel::Ch4>},
                                      {"set_angle_5", command_tasks::set_angle_task<bsd::pwm_channel::Ch5>},
                                      {"set_angle_6", command_tasks::set_angle_task<bsd::pwm_channel::Ch6>},
                                      {"set_angle_7", command_tasks::set_angle_task<bsd::pwm_channel::Ch7>},
                                      {"set_angle_8", command_tasks::set_angle_task<bsd::pwm_channel::Ch8>},
                                      {"store_pwm_value", command_tasks::store_pwm_value},
                                      {"update_pwm_value", command_tasks::update_pwm_value},
                                      {"switch", command_tasks::switch_on},
                                      {"rotate", command_tasks::rotate},
                                      {"grasp_ratio", command_tasks::grasp_ratio},
                                  });
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
    base::log("Store Duty{ ch1: ", DUTY(0), ", ch2: ", DUTY(1), ", ch3: ", DUTY(2), ", ch4: ", DUTY(3),
              ", ch5: ", DUTY(4), ", ch6: ", DUTY(5), ", ch7: ", DUTY(6), ", ch8: ", DUTY(7), " }");
#undef DUTY
    taskEXIT_CRITICAL();

    osThreadTerminate(osThreadGetId());
}
