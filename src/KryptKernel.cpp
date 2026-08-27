#include "KryptKernel.h"

namespace Krypton {

    Kernel SystemKernel;

    Kernel::Kernel() : bootCompleted(false), bootTimestamp(0) {}

    Kernel::~Kernel() {}

    bool Kernel::boot() {
        Serial.begin(115200);
        delay(300);

        Serial.println("\n==================================================");
        Serial.println("   KRYPTKERNEL v" KRYPT_KERNEL_VERSION " - ARCHITECTURE INIT   ");
        Serial.println("==================================================\n");

        memoryManager.init();

        if (!scheduler.init()) {
            panic("TaskScheduler mutex initialization failed!");
            return false;
        }

        if (!vfs.mount("/sys", true)) {
            Serial.println("[WARN] VFS mount failed. Running in memory-only mode.");
        }

        bootTimestamp = millis();
        bootCompleted = true;

        Serial.println("[BOOT] KryptKernel online and ready.\n");
        return true;
    }

    void Kernel::panic(const char* reason) {
        Serial.println("\n**************************************************");
        Serial.println("              !!! KERNEL PANIC !!!                ");
        Serial.printf(" Reason: %s\n", reason);
        Serial.println(" System restarting in 3 seconds...");
        Serial.println("**************************************************\n");
        delay(3000);
        ESP.restart();
    }

    void Kernel::printSystemReport() {
        Serial.println("\n================ [SYSTEM REPORT] ================");
        Serial.printf(" Kernel Version: %s\n", KRYPT_KERNEL_VERSION);
        Serial.printf(" Uptime: %lu ms\n", millis() - bootTimestamp);
        Serial.printf(" CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
        Serial.printf(" Free Heap: %u bytes\n", memoryManager.getMemorySnapshot().freeHeap);
        if (vfs.isMounted()) {
            Serial.printf(" VFS Used: %u / %u bytes\n", vfs.getUsedBytes(), vfs.getTotalBytes());
        }
        Serial.println("=================================================\n");
    }

} // namespace Krypton
