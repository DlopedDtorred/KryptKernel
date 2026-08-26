#ifndef KRYPT_FS_H
#define KRYPT_FS_H

#include <Arduino.h>
#include <LittleFS.h>
#include "KryptConfig.h"

class KryptFS {
public:
  bool init() {
    return LittleFS.begin(true);
  }

  String resolvePath(String currentPath, String target) {
    if (target == "") return currentPath;
    if (target.startsWith("/")) return target;
    if (currentPath == "/") return "/" + target;
    return currentPath + "/" + target;
  }

  bool exists(String path) {
    return LittleFS.exists(path);
  }

  bool makeDirectory(String path) {
    return LittleFS.mkdir(path);
  }

  bool removeEntry(String path) {
    return LittleFS.rmdir(path) || LittleFS.remove(path);
  }
};

#endif
