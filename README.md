# ⚙️ Krypt Kernel

> **Krypt** es un núcleo monolítico ligero, modular y portable diseñado sobre FreeRTOS y LittleFS para microcontroladores ESP32.

![Architecture](https://img.shields.io/badge/Type-Monolithic%20Kernel-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32%20%2F%20FreeRTOS-green)
![License](https://img.shields.io/badge/License-MIT-blue)

---

## 🏛️ Arquitectura del Kernel

`Krypt` ofrece una capa de abstracción limpia entre el hardware de la ESP32 y cualquier sistema operativo o distribución de nivel superior (como *KryptonOS*).

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
## 🧩 Módulos del Kernel

    KryptHAL (Hardware Abstraction Layer): Control de bajo nivel del hardware, lecturas del chip (frecuencia, cores, RAM libre), reinicios del sistema y gestión persistente mediante NVS (Non-Volatile Storage).

    KryptFS (FileSystem Driver): Driver de almacenamiento basado en LittleFS con resolución de rutas relativas y absolutas, borrado seguro y creación de directorios.

    KryptTask (Task Manager): Wrapper asíncrono para spawnear y gestionar tareas/procesos independientes sobre FreeRTOS.

    KryptNet (Network Engine): Pila de red WiFi y motor HTTP para peticiones salientes.

## 📁 Estructura del Repositorio
Plaintext

KryptKernel/
├── Krypt.h           # Interfaz pública unificada del Kernel
├── KryptConfig.h     # Definiciones de versión y macros globales
├── KryptHAL.h        # Abstracción de hardware y NVS
├── KryptFS.h         # Gestor del sistema de archivos LittleFS
├── KryptTask.h       # Scheduler de procesos FreeRTOS
├── KryptNet.h        # Servicio de red y socket HTTP
├── library.json      # Manifest para PlatformIO
├── library.properties# Manifest para Arduino IDE
└── README.md

## 💻 Ejemplo de Uso (Integration API)
C++
```bash
#include <Arduino.h>
#include <Krypt.h>

void setup() {
  Serial.begin(115200);

  // Inicializar el Kernel
  if (Krypt.boot()) {
    Serial.println("[Krypt] Kernel iniciado correctamente.");
  }

  // Uso de KryptHAL
  Serial.printf("RAM Libre: %d KB\n", Krypt.hal.getFreeRAM() / 1024);

  // Uso de KryptFS
  Krypt.fs.makeDirectory("/sys");

  // Guardar datos persistentes NVS
  Krypt.hal.setKeyString("system", "hostname", "krypt-node");
}

void loop() {
  // El Kernel administra las tareas en background
}
``` 
## 📜 Licencia

Este proyecto se distribuye bajo la licencia MIT.


