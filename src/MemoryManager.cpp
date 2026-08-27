#include "MemoryManager.h"

namespace Krypton {

    MemoryManager::MemoryManager() {}

    void MemoryManager::init() {
        Serial.printf("[MEM] Internal Heap: %u bytes | PSRAM Present: %s\n", 
                      ESP.getHeapSize(), psramFound() ? "YES" : "NO");
    }

    MemoryInfo MemoryManager::getMemorySnapshot() {
        MemoryInfo info;
        info.totalHeap = ESP.getHeapSize();
        info.freeHeap = ESP.getFreeHeap();
        info.minFreeHeap = ESP.getMinFreeHeap();
        info.maxAllocBlock = ESP.getMaxAllocHeap();
        info.psramAvailable = psramFound();
        info.freePsram = info.psramAvailable ? ESP.getFreePsram() : 0;
        return info;
    }

    void MemoryManager::printMemoryReport() {
        MemoryInfo info = getMemorySnapshot();
        Serial.println("\n--- [KryptKernel Memory Status] ---");
        Serial.printf(" Free Heap: %u / %u bytes\n", info.freeHeap, info.totalHeap);
        Serial.printf(" Min Free Heap Ever: %u bytes\n", info.minFreeHeap);
        Serial.printf(" Max Allocatable Block: %u bytes\n", info.maxAllocBlock);
        if (info.psramAvailable) {
            Serial.printf(" PSRAM Free: %u bytes\n", info.freePsram);
        }
        Serial.println("------------------------------------\n");
    }

    void* MemoryManager::allocateInternal(size_t size) {
        return heap_caps_malloc(size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    }

    void* MemoryManager::allocatePSRAM(size_t size) {
        if (!psramFound()) return nullptr;
        return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    }

    void MemoryManager::freeMemory(void* ptr) {
        if (ptr != nullptr) free(ptr);
    }

    bool MemoryManager::isMemoryHealthy(size_t minThresholdBytes) {
        return (ESP.getFreeHeap() >= minThresholdBytes);
    }

} // namespace Krypton
