#pragma once
#include <sstream>
#include <type_traits>
#include <utility>

#include "debug_printf.h"

namespace base
{

template <typename Type> void _logf(std::stringstream &ss, Type &&t)
{
    static_assert(
        not std::is_same_v<typename std::remove_reference<typename std::remove_cv<Type>::type>::type, float> and
        not std::is_same_v<typename std::remove_reference<typename std::remove_cv<Type>::type>::type, double>);

    ss << std::forward<Type &&>(t) << "\n";
}

template <typename Type, typename... _Type> void _logf(std::stringstream &ss, Type &&t, _Type &&...t1)
{
    static_assert(
        not std::is_same_v<typename std::remove_reference<typename std::remove_cv<Type>::type>::type, float> and
        not std::is_same_v<typename std::remove_reference<typename std::remove_cv<Type>::type>::type, double>);
    ss << t;
    _logf(ss, std::forward<_Type &&>(t1)...);
}

template <typename... Type> void log(Type &&...t)
{
    std::stringstream ss;
    _logf(ss, std::forward<Type &&>(t)...);
    log_to_serial(ss.str().c_str());
}

template <typename... Type> void print(Type &&...t)
{
    std::stringstream ss;
    _logf(ss, std::forward<Type &&>(t)...);
    print_to_serial(ss.str().c_str());
}

} // namespace base
