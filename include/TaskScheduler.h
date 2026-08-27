#ifndef KRYPT_TASK_SCHEDULER_H
#define KRYPT_TASK_SCHEDULER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

namespace Krypton {

    enum class TaskPriority : UBaseType_t {
        IDLE = 0,
        LOW = 1,
        NORMAL = 2,
        HIGH = 3,
        CRITICAL = 4,
        REALTIME = 5
    };

    class TaskScheduler {
    private:
        SemaphoreHandle_t schedulerLock;

    public:
        TaskScheduler();
        ~TaskScheduler();

        bool init();

        // Asigna tareas directamente a PRO_CPU (Core 0) o APP_CPU (Core 1)
        bool createTask(TaskFunction_t taskFunc, const char* name, uint32_t stackSize, 
                        void* param, TaskPriority priority, uint8_t coreID, TaskHandle_t* outHandle = nullptr);

        void suspendTask(TaskHandle_t handle);
        void resumeTask(TaskHandle_t handle);
        void deleteTask(TaskHandle_t handle);

        uint32_t getStackHighWaterMark(TaskHandle_t handle);
    };

} // namespace Krypton

#endif // KRYPT_TASK_SCHEDULER_H
