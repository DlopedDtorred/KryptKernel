# ⚙️ Krypt Kernel

> **Krypt** is a lightweight, modular, and portable monolithic kernel designed for ESP32 microcontrollers using FreeRTOS and LittleFS.

![Architecture](https://img.shields.io/badge/Type-Monolithic%20Kernel-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32%20%2F%20FreeRTOS-green)
![License](https://img.shields.io/badge/License-MIT-blue)

---

## 🏛️ Kernel Architecture

`Krypt` provides a clean abstraction layer between ESP32 hardware and any higher-level operating system or distribution (such as *KryptonOS*).

```text
+-------------------------------------------------------+
|            User Space / Distro (e.g. KnotOS)       |
+-------------------------------------------------------+
                           |  (System API)
                           v
+-------------------------------------------------------+
|                    Krypt Kernel                       |
|  +--------------+  +--------------+  +-------------+  |
|  |   KryptHAL   |  |   KryptFS    |  |  KryptNet   |  |
|  +--------------+  +--------------+  +-------------+  |
|  |                  KryptTask                      |  |
|  +-------------------------------------------------+  |
+-------------------------------------------------------+
                           |
                           v
+-------------------------------------------------------+
|                 Hardware / FreeRTOS                   |
+-------------------------------------------------------+
``` 
## 🧩 Kernel Modules

    MemoryManager: Internal heap and PSRAM allocation, metrics, and health checks.

    VFS (FileSystem Driver): LittleFS-based storage driver with mounting, file I/O, and safe deletion.

    TaskScheduler (Task Manager): Wrapper for spawning and managing independent FreeRTOS tasks.


## 📁 Repository Structure
```Plaintext

KryptKernel/
├── include/KryptKernel.h    # Unified public Kernel interface
├── include/MemoryManager.h  # Heap and PSRAM manager
├── include/VFS.h            # LittleFS file system manager
├── include/TaskScheduler.h  # FreeRTOS task scheduler
├── library.json      # PlatformIO manifest
└── README.md
``` 
## 💻 Usage Example (Integration API)
C++
```bash
#include <Arduino.h>
#include <KryptKernel.h>

void setup() {
  if (Krypton::SystemKernel.boot()) {
    Serial.println("[Krypt] Kernel started successfully.");
  }

  // Use the memory manager
  Serial.printf("Free RAM: %u KB\n",
                Krypton::SystemKernel.getMemoryManager().getMemorySnapshot().freeHeap / 1024);

  // Use the VFS
  Krypton::SystemKernel.getVFS().writeFile("/sys/boot.log", "Kernel started");
}

void loop() {
  // The Kernel manages background tasks
}
``` 
## 📜 License

This project is distributed under the MIT License.

## KryptonOS integration

KryptonOS consumes this repository as the `KryptKernel` PlatformIO library.
The kernel owns the global `Krypton::SystemKernel` instance and initializes
the scheduler, memory manager, and LittleFS VFS during `boot()`.

The VFS is mounted at `/sys`; paths passed to `Krypton::VFS` may be written
as `/sys/file.txt` or `/file.txt`. File and directory operations are
serialized with an internal FreeRTOS mutex so applications can share the VFS
with the shell.

Mounting, unmounting, and capacity queries use the same lock, so filesystem
diagnostics can safely run while an application task accesses the VFS.
The public API and task priority names remain backward-compatible with
KnotOS/KryptonOS integrations.

KryptonOS creates its shell, web, and application tasks through
`Krypton::TaskScheduler`. The scheduler priority names are `Low`, `Medium`,
`Normal`, `High`, and `Critical` to avoid collisions with Arduino's `LOW` and
`HIGH` macros.
