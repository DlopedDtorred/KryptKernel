#include "VFS.h"

namespace Krypton {

    VFS::VFS() : mounted(false) {
        memset(mountPath, 0, sizeof(mountPath));
    }

    bool VFS::mount(const char* path, bool formatOnFail) {
        strncpy(mountPath, path, sizeof(mountPath) - 1);
        mounted = LittleFS.begin(formatOnFail, mountPath);
        if (mounted) {
            Serial.printf("[VFS] Virtual File System mounted successfully at '%s'\n", mountPath);
        } else {
            Serial.printf("[VFS] ERROR: Failed to mount VFS at '%s'\n", mountPath);
        }
        return mounted;
    }

    void VFS::unmount() {
        if (mounted) {
            LittleFS.end();
            mounted = false;
            Serial.println("[VFS] Unmounted.");
        }
    }

    bool VFS::exists(const char* filepath) {
        if (!mounted) return false;
        return LittleFS.exists(filepath);
    }

    bool VFS::writeFile(const char* filepath, const char* content) {
        if (!mounted) return false;
        File f = LittleFS.open(filepath, FILE_WRITE);
        if (!f) return false;
        size_t bytesWritten = f.print(content);
        f.close();
        return (bytesWritten > 0);
    }

    String VFS::readFile(const char* filepath) {
        if (!mounted || !exists(filepath)) return "";
        File f = LittleFS.open(filepath, FILE_READ);
        if (!f) return "";
        String content = f.readString();
        f.close();
        return content;
    }

    bool VFS::deleteFile(const char* filepath) {
        if (!mounted) return false;
        return LittleFS.remove(filepath);
    }

    size_t VFS::getTotalBytes() {
        return mounted ? LittleFS.totalBytes() : 0;
    }

    size_t VFS::getUsedBytes() {
        return mounted ? LittleFS.usedBytes() : 0;
    }

} // namespace Krypton
