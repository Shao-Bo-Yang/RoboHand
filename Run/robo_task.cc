#include "robo_task.h"

void RoboTaskHelperFunc(void* _arg) {
  void** arg = (void**)_arg;
  typedef void (*Func)(void*);
  Func func = (Func)arg[0];
  void* obj = (void*)arg[1];
  func(obj);
}
