#pragma once

#include <unordered_map>

#include "cmsis_os2.h"
#include "noti_center.h"
namespace base {

struct noti_center {
 public:
  static noti_center &instance() {
    static noti_center instance{};
    return instance;
  }

 public:
  bool register_with_type(osThreadId_t thread_id, noti_type type);
  osThreadId_t thread_id_with_type(noti_type type);
  bool noti_with_type(noti_type type);
  void wait_for_type(osThreadId_t thread_id, noti_type type);

 private:
  noti_center() = default;
  ~noti_center() = default;
  std::unordered_map<noti_type, osThreadId_t> _mapping{};
};

}  // namespace base