#pragma once

#include <cstdint>
#include <memory>

#include "macro.h"
#include <cmsis_os2.h>

namespace base
{

struct rw_lock
{
    rw_lock() : _m_lock(std::make_unique<_M_rw_lock>())
    {
        _m_lock->rd_count = 0;
        osMutexAttr_t attr{};
        attr.attr_bits |= osMutexRecursive;
        _m_lock->rd_count_lock = osMutexNew(&attr);
        _m_lock->wr_lock = osMutexNew(&attr);
    }

    rw_lock(rw_lock &&other) : _m_lock(nullptr)
    {
        _m_lock.swap(other._m_lock);
    }

    rw_lock &operator=(rw_lock &&other)
    {
        _m_lock.swap(other._m_lock);
        other._m_lock = nullptr;
        return *this;
    }

    ~rw_lock()
    {
        if (_m_lock)
        {
            osMutexDelete(_m_lock->wr_lock);
            osMutexDelete(_m_lock->rd_count_lock);
            _m_lock->rd_count = 0;
        }
    }

  public:
    enum class rw_lock_operation
    {
        Lock,
        Unlock,
    };

    template <rw_lock_operation op> auto read_operate(uint32_t wait)
    {
        osStatus_t state = osMutexAcquire(_m_lock->rd_count_lock, wait);
        if (state == osOK)
        {
            if constexpr (op == rw_lock_operation::Lock)
            {
                if (_m_lock->rd_count++ == 1)
                {
                    state = osMutexAcquire(_m_lock->wr_lock, wait);
                }
            }
            else if (op == rw_lock_operation::Unlock)
            {
                if (_m_lock->rd_count-- == 0)
                {
                    osMutexRelease(_m_lock->wr_lock);
                }
            }
            osMutexRelease(_m_lock->rd_count_lock);
        }
        return state;
    }
    template <rw_lock_operation op> auto write_operate(uint32_t wait)
    {
        osStatus_t state = osOK;
        if constexpr (op == rw_lock_operation::Lock)
        {
            state = osSemaphoreAcquire(_m_lock->wr_lock, wait);
        }
        else if (op == rw_lock_operation::Unlock)
        {
            osSemaphoreRelease(_m_lock->wr_lock);
        }
        return state;
    }

  private:
    struct _M_rw_lock
    {
        osMutexId_t rd_count_lock;
        osMutexId_t wr_lock;
        uint32_t rd_count;
    };

  private:
    std::unique_ptr<_M_rw_lock> _m_lock;

  private:
    rw_lock(const rw_lock &) = delete;
    rw_lock &operator=(const rw_lock &) = delete;
};

} // namespace base
