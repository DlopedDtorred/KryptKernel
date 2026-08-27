#include <Arduino.h>
#include <KryptKernel.h>

void userTaskCore0(void* pvParameters) {
    for (;;) {
        Serial.println("[TaskCore0] Ejecutando en PRO_CPU (Core 0)...");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void userTaskCore1(void* pvParameters) {
    for (;;) {
        Serial.println("[TaskCore1] Ejecutando en APP_CPU (Core 1)...");
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void setup() {
    // Inicializar el Kernel
    if (!Krypton::SystemKernel.boot()) {
        return;
    }

    // Probar VFS (Escritura y Lectura)
    Krypton::SystemKernel.getVFS().writeFile("/sys/boot.log", "Kernel v2.0 Boot Successful");
    String log = Krypton::SystemKernel.getVFS().readFile("/sys/boot.log");
    Serial.printf("[VFS Log Test]: %s\n", log.c_str());

    // Crear Tareas Multinúcleo
    Krypton::SystemKernel.getScheduler().createTask(
        userTaskCore0, "WorkerCore0", 2048, NULL, Krypton::TaskPriority::NORMAL, 0
    );

    Krypton::SystemKernel.getScheduler().createTask(
        userTaskCore1, "WorkerCore1", 2048, NULL, Krypton::TaskPriority::NORMAL, 1
    );

    // Imprimir Reporte de Diagnóstico
    Krypton::SystemKernel.printSystemReport();
}

void loop() {
    // El loop queda libre para tareas secundarias o monitoreo
    vTaskDelay(pdMS_TO_TICKS(5000));
    Krypton::SystemKernel.getMemoryManager().printMemoryReport();
}
