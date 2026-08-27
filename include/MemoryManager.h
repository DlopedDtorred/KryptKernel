#ifndef KRYPT_MEMORY_MANAGER_H
#define KRYPT_MEMORY_MANAGER_H

#include <Arduino.h>
#include <esp_heap_caps.h>

namespace Krypton {

    struct MemoryInfo {
        size_t totalHeap;
        size_t freeHeap;
        size_t minFreeHeap;
        size_t maxAllocBlock;
        bool psramAvailable;
        size_t freePsram;
    };

    class MemoryManager {
    public:
        MemoryManager();

        void init();

        MemoryInfo getMemorySnapshot();
        void printMemoryReport();

        void* allocateInternal(size_t size);
        void* allocatePSRAM(size_t size);
        void freeMemory(void* ptr);

        bool isMemoryHealthy(size_t minThresholdBytes = 16384);
    };

} // namespace Krypton

#endif // KRYPT_MEMORY_MANAGER_H
