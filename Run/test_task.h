#pragma once

#include <list>
#include <optional>
#include <string>

#include "cmsis_os2.h"
#include "robo_task.h"
namespace task {

struct test_task : public task_generic<test_task> {
 public:
  void operator()();

 private:
  test_task() = default;
  ~test_task() = default;

 private:
  friend task_generic;
};

}  // namespace task