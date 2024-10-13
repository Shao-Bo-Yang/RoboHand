#pragma once

#include <cstdint>

#include "bsd_getter.h"
namespace bsd
{

using flash_data_t = uint32_t;

struct flash : public bsd_getter<flash>
{
  public:
    void init();

  public:
    void read_flash(flash_data_t *data, uint32_t length);

    void write_flash(const flash_data_t *data, uint32_t length);

  private:
    flash();
    virtual ~flash() = default;
    flash(const flash &) = delete;
    flash &operator=(const flash &) = delete;

  private:
    friend bsd_getter;
};

} // namespace bsd
