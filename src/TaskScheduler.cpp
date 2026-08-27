#include "TaskScheduler.h"

namespace Krypton {

    TaskScheduler::TaskScheduler() : schedulerLock(NULL) {}

    TaskScheduler::~TaskScheduler() {
        if (schedulerLock != NULL) {
            vSemaphoreDelete(schedulerLock);
        }
    }

    bool TaskScheduler::init() {
        schedulerLock = xSemaphoreCreateMutex();
        return (schedulerLock != NULL);
    }

    bool TaskScheduler::createTask(TaskFunction_t taskFunc, const char* name, uint32_t stackSize, 
                                   void* param, TaskPriority priority, uint8_t coreID, TaskHandle_t* outHandle) {
        if (coreID > 1) coreID = 1;

        BaseType_t res = xTaskCreatePinnedToCore(
            taskFunc,
            name,
            stackSize,
            param,
            static_cast<UBaseType_t>(priority),
            outHandle,
            coreID
        );

        if (res == pdPASS) {
            Serial.printf("[SCHEDULER] Task '%s' spawned on CORE %d (Stack: %u bytes)\n", name, coreID, stackSize);
            return true;
        } else {
            Serial.printf("[SCHEDULER] ERROR: Failed to create task '%s'\n", name);
            return false;
        }
    }

    void TaskScheduler::suspendTask(TaskHandle_t handle) {
        if (handle != NULL) vTaskSuspend(handle);
    }

    void TaskScheduler::resumeTask(TaskHandle_t handle) {
        if (handle != NULL) vTaskResume(handle);
    }

    void TaskScheduler::deleteTask(TaskHandle_t handle) {
        if (handle != NULL) vTaskDelete(handle);
    }

    uint32_t TaskScheduler::getStackHighWaterMark(TaskHandle_t handle) {
        return (handle != NULL) ? uxTaskGetStackHighWaterMark(handle) : 0;
    }

} // namespace Krypton
