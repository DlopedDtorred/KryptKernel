#ifndef KRYPT_KERNEL_TASK_SCHEDULER_H
#define KRYPT_KERNEL_TASK_SCHEDULER_H

#include <Arduino.h>

// LOW and HIGH are Arduino GPIO macros. Preserve them for users after this
// header is included while still allowing the scoped enum members below.
#ifdef LOW
  #pragma push_macro("LOW")
  #undef LOW
  #define KRYPT_RESTORE_LOW
#endif

#ifdef HIGH
  #pragma push_macro("HIGH")
  #undef HIGH
  #define KRYPT_RESTORE_HIGH
#endif

namespace Krypton {

    enum class TaskPriority : UBaseType_t {
        IDLE = 0,
        Low = 1,
        MEDIUM = 2,
        NORMAL = MEDIUM,
        High = 3,
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

#ifdef KRYPT_RESTORE_HIGH
  #pragma pop_macro("HIGH")
  #undef KRYPT_RESTORE_HIGH
#endif
#ifdef KRYPT_RESTORE_LOW
  #pragma pop_macro("LOW")
  #undef KRYPT_RESTORE_LOW
#endif

#endif // KRYPT_KERNEL_TASK_SCHEDULER_H
