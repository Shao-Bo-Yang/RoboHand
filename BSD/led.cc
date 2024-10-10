#include "led.h"

#include <cstddef>

#include "bsd_getter.h"
#include "cmsis_os2.h"

namespace bsd {

led::led() : bsd_getter() {}

void led::init() {}

}  // namespace bsd
