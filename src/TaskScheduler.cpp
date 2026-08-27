#include "TaskScheduler.h"

namespace Krypton {

    TaskScheduler::TaskScheduler() : schedulerLock(nullptr) {}

    TaskScheduler::~TaskScheduler() {
        if (schedulerLock != nullptr) {
            vSemaphoreDelete(schedulerLock);
            schedulerLock = nullptr;
        }
    }

    bool TaskScheduler::init() {
        if (schedulerLock != nullptr) {
            return true;
        }
        schedulerLock = xSemaphoreCreateMutex();
        return (schedulerLock != NULL);
    }

    bool TaskScheduler::createTask(TaskFunction_t taskFunc, const char* name, uint32_t stackSize,
                                   void* param, TaskPriority priority, uint8_t coreID, TaskHandle_t* outHandle) {
        if (taskFunc == nullptr || name == nullptr || stackSize == 0) {
            return false;
        }

        // ESP32-S2/S3/C3 variants may have only one core. Never pin to a
        // non-existent core when code written for a dual-core ESP32 is reused.
        if (coreID >= portNUM_PROCESSORS) {
            coreID = static_cast<uint8_t>(portNUM_PROCESSORS - 1);
        }

        if (schedulerLock != nullptr) {
            xSemaphoreTake(schedulerLock, portMAX_DELAY);
        }

        BaseType_t res = xTaskCreatePinnedToCore(
            taskFunc,
            name,
            stackSize,
            param,
            static_cast<UBaseType_t>(priority),
            outHandle,
            coreID
        );

        if (schedulerLock != nullptr) {
            xSemaphoreGive(schedulerLock);
        }

        if (res == pdPASS) {
            Serial.printf("[SCHEDULER] Task '%s' spawned on CORE %u (Stack: %u words)\n",
                          name, static_cast<unsigned>(coreID), static_cast<unsigned>(stackSize));
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
