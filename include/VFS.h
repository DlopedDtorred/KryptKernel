#ifndef KRYPT_VFS_H
#define KRYPT_VFS_H

#include <Arduino.h>
#include <LittleFS.h>

namespace Krypton {

    class VFS {
    private:
        bool mounted;
        char mountPath[32];

    public:
        VFS();

        bool mount(const char* path = "/sys", bool formatOnFail = true);
        void unmount();

        bool exists(const char* filepath);
        bool writeFile(const char* filepath, const char* content);
        String readFile(const char* filepath);
        bool deleteFile(const char* filepath);

        size_t getTotalBytes();
        size_t getUsedBytes();

        bool isMounted() const { return mounted; }
    };

} // namespace Krypton

#endif // KRYPT_VFS_H
