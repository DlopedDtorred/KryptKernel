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
|            User Space / Distro (e.g. KryptonOS)       |
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

    KryptHAL (Hardware Abstraction Layer): Low-level hardware control, chip metrics (frequency, cores, free RAM), system resets, and persistent storage management through NVS (Non-Volatile Storage).

    KryptFS (FileSystem Driver): LittleFS-based storage driver with relative and absolute path resolution, safe deletion, and directory creation.

    KryptTask (Task Manager): Asynchronous wrapper for spawning and managing independent tasks/processes on FreeRTOS.

    KryptNet (Network Engine): Wi-Fi network stack and HTTP engine for outbound requests.

## 📁 Repository Structure
```Plaintext

KryptKernel/
├── Krypt.h           # Unified public Kernel interface
├── KryptConfig.h     # Version definitions and global macros
├── KryptHAL.h        # Hardware and NVS abstraction
├── KryptFS.h         # LittleFS file system manager
├── KryptTask.h       # FreeRTOS process scheduler
├── KryptNet.h        # Network service and HTTP socket
├── library.json      # PlatformIO manifest
├── library.properties# Arduino IDE manifest
└── README.md
``` 
## 💻 Usage Example (Integration API)
C++
```bash
#include <Arduino.h>
#include <Krypt.h>

void setup() {
  Serial.begin(115200);

  // Initialize the Kernel
  if (Krypt.boot()) {
    Serial.println("[Krypt] Kernel started successfully.");
  }

  // Use KryptHAL
  Serial.printf("Free RAM: %d KB\n", Krypt.hal.getFreeRAM() / 1024);

  // Use KryptFS
  Krypt.fs.makeDirectory("/sys");

  // Save persistent NVS data
  Krypt.hal.setKeyString("system", "hostname", "krypt-node");
}

void loop() {
  // The Kernel manages background tasks
}
``` 
## 📜 License

This project is distributed under the MIT License.

