#ifndef KRYPT_KERNEL_TASK_SCHEDULER_H
#define KRYPT_KERNEL_TASK_SCHEDULER_H

#include <Arduino.h>

// -----------------------------------------------------------------------------
// Solución al conflicto con la HAL de Arduino:
// Arduino.h define macros globales '#define LOW 0x0' y '#define HIGH 0x1'.
// Se desdefinen temporalmente para no romper la declaración del enum TaskPriority.
// -----------------------------------------------------------------------------
#ifdef LOW
  #undef LOW
#endif

#ifdef HIGH
  #undef HIGH
#endif

namespace Krypton {

    enum class TaskPriority : UBaseType_t {
        IDLE = 0,
        LOW = 1,
        MEDIUM = 2,
        HIGH = 3,
        CRITICAL = 4
    };

    class TaskScheduler {
    private:
        SemaphoreHandle_t schedulerLock;

    public:
        TaskScheduler();
        ~TaskScheduler();

        bool init();

        bool createTask(TaskFunction_t taskFunc,
                        const char* name,
                        uint32_t stackDepth,
                        void* param,
                        TaskPriority priority,
                        uint8_t coreID,
                        TaskHandle_t* outHandle = nullptr);

        void suspendTask(TaskHandle_t handle);
        void resumeTask(TaskHandle_t handle);
        void deleteTask(TaskHandle_t handle);

        uint32_t getStackHighWaterMark(TaskHandle_t handle);
    };

} // namespace Krypton

// Opcional: Si los usuarios del Kernel necesitan las macros de Arduino GPIO 
// más adelante en el mismo archivo, se pueden redefinir al final:
#ifndef LOW
  #define LOW 0x0
#endif
#ifndef HIGH
  #define HIGH 0x1
#endif

#endif // KRYPT_KERNEL_TASK_SCHEDULER_H
