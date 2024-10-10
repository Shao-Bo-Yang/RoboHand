#pragma once

#include <string>

#include "bsd_getter.h"
namespace bsd {

struct usart6 : public bsd_getter<usart6> {
 public:
  void init();

 public:
  void write(std::string& str);

 private:

 private:
  usart6();
  ~usart6() = default;
  usart6(const usart6&) = delete;
  usart6& operator=(const usart6&) = delete;

  friend bsd_getter;
};

}  // namespace bsd