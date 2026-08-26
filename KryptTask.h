#ifndef KRYPT_TASK_H
#define KRYPT_TASK_H

#include <Arduino.h>
#include "KryptConfig.h"

class KryptTask {
public:
  // Crear una tarea de FreeRTOS
  static bool createProcess(TaskFunction_t taskFunc, const char* name, uint32_t stackSize = 4096, void* param = NULL, UBaseType_t priority = 1) {
    BaseType_t result = xTaskCreate(taskFunc, name, stackSize, param, priority, NULL);
    return (result == pdPASS);
  }
};

#endif
