#pragma once

#include <FreeRTOS.h>
#include <task.h>

#include <string>

#include "bsd_getter.h"
namespace bsd
{

struct switch_on : public bsd_getter<switch_on>
{
  public:
    void init();

  public:
    void on(void);
    void off(void);
    bool is_on(void);

  private:
    switch_on();
    ~switch_on() = default;
    switch_on(const switch_on &) = delete;
    switch_on &operator=(const switch_on &) = delete;

    friend bsd_getter;
};

} // namespace bsd
