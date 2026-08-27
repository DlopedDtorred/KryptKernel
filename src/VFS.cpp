#include "VFS.h"

namespace Krypton {

    namespace {
        String filesystemPath(const char* filepath, const char* mountPath) {
            if (filepath == nullptr) return String();
            const size_t mountLength = strlen(mountPath);
            if (strncmp(filepath, mountPath, mountLength) == 0 &&
                (filepath[mountLength] == '\0' || filepath[mountLength] == '/')) {
                const char* relative = filepath + mountLength;
                return relative[0] == '\0' ? String("/") : String(relative);
            }
            return String(filepath);
        }
    }

    VFS::VFS() : mounted(false), lock(xSemaphoreCreateMutex()) {
        memset(mountPath, 0, sizeof(mountPath));
    }

    VFS::~VFS() {
        unmount();
        if (lock != nullptr) vSemaphoreDelete(lock);
    }

    bool VFS::mount(const char* path, bool formatOnFail) {
        if (path == nullptr || path[0] == '\0') return false;
        if (mounted) unmount();
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        strncpy(mountPath, path, sizeof(mountPath) - 1);
        mountPath[sizeof(mountPath) - 1] = '\0';
        mounted = LittleFS.begin(formatOnFail, mountPath);
        if (mounted) {
            Serial.printf("[VFS] Virtual File System mounted successfully at '%s'\n", mountPath);
        } else {
            Serial.printf("[VFS] ERROR: Failed to mount VFS at '%s'\n", mountPath);
        }
        if (lock != nullptr) xSemaphoreGive(lock);
        return mounted;
    }

    void VFS::unmount() {
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        if (mounted) {
            LittleFS.end();
            mounted = false;
            Serial.println("[VFS] Unmounted.");
        }
        if (lock != nullptr) xSemaphoreGive(lock);
    }

    bool VFS::exists(const char* filepath) {
        if (!mounted || filepath == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(filepath, mountPath);
        const bool result = LittleFS.exists(path.c_str());
        if (lock != nullptr) xSemaphoreGive(lock);
        return result;
    }

    bool VFS::writeFile(const char* filepath, const char* content) {
        if (!mounted || filepath == nullptr || content == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(filepath, mountPath);
        File f = LittleFS.open(path.c_str(), FILE_WRITE);
        if (!f) {
            if (lock != nullptr) xSemaphoreGive(lock);
            return false;
        }
        size_t bytesWritten = f.write(reinterpret_cast<const uint8_t*>(content), strlen(content));
        f.close();
        if (lock != nullptr) xSemaphoreGive(lock);
        return (bytesWritten == strlen(content));
    }

    String VFS::readFile(const char* filepath) {
        if (!mounted || filepath == nullptr || !exists(filepath)) return "";
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(filepath, mountPath);
        File f = LittleFS.open(path.c_str(), FILE_READ);
        if (!f) {
            if (lock != nullptr) xSemaphoreGive(lock);
            return "";
        }
        String content = f.readString();
        f.close();
        if (lock != nullptr) xSemaphoreGive(lock);
        return content;
    }

    bool VFS::deleteFile(const char* filepath) {
        if (!mounted || filepath == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(filepath, mountPath);
        const bool result = LittleFS.remove(path.c_str());
        if (lock != nullptr) xSemaphoreGive(lock);
        return result;
    }

    bool VFS::listDirectory(const char* directory) {
        if (!mounted || directory == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(directory, mountPath);
        File root = LittleFS.open(path.c_str());
        if (!root || !root.isDirectory()) {
            if (root) root.close();
            if (lock != nullptr) xSemaphoreGive(lock);
            return false;
        }
        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                Serial.printf("  <DIR> %s\n", file.name());
            } else {
                Serial.printf("  %8u B  %s\n", (unsigned int)file.size(), file.name());
            }
            file.close();
            file = root.openNextFile();
        }
        root.close();
        if (lock != nullptr) xSemaphoreGive(lock);
        return true;
    }

    bool VFS::createDirectory(const char* directory) {
        if (!mounted || directory == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String path = filesystemPath(directory, mountPath);
        const bool result = LittleFS.mkdir(path.c_str());
        if (lock != nullptr) xSemaphoreGive(lock);
        return result;
    }

    bool VFS::removeEntry(const char* path) {
        if (!mounted || path == nullptr) return false;
        if (lock != nullptr) xSemaphoreTake(lock, portMAX_DELAY);
        const String resolved = filesystemPath(path, mountPath);
        const bool result = LittleFS.remove(resolved.c_str()) || LittleFS.rmdir(resolved.c_str());
        if (lock != nullptr) xSemaphoreGive(lock);
        return result;
    }

    size_t VFS::getTotalBytes() {
        return mounted ? LittleFS.totalBytes() : 0;
    }

    size_t VFS::getUsedBytes() {
        return mounted ? LittleFS.usedBytes() : 0;
    }

} // namespace Krypton
