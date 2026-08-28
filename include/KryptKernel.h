#ifndef KRYPT_KERNEL_H
#define KRYPT_KERNEL_H

#include <Arduino.h>
#include "TaskScheduler.h"
#include "MemoryManager.h"
#include "VFS.h"

#define KRYPT_KERNEL_VERSION "2.0.1"

namespace Krypton {

    class Kernel {
    private:
        bool bootCompleted;
        uint32_t bootTimestamp;

        TaskScheduler scheduler;
        MemoryManager memoryManager;
        VFS vfs;

    public:
        Kernel();
        ~Kernel();

        // Boot and Kernel lifecycle
        bool boot();
        void panic(const char* reason);

        // Core Subsystem Accessors
        TaskScheduler& getScheduler() { return scheduler; }
        MemoryManager& getMemoryManager() { return memoryManager; }
        VFS& getVFS() { return vfs; }

        // Diagnostics
        void printSystemReport();
    };

    extern Kernel SystemKernel;

} // namespace Krypton

#endif // KRYPT_KERNEL_H
