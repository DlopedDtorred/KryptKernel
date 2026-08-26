#ifndef KRYPT_H
#define KRYPT_H

#include "KryptConfig.h"
#include "KryptHAL.h"
#include "KryptFS.h"
#include "KryptTask.h"
#include "KryptNet.h"

class KryptKernel {
public:
  KryptHAL hal;
  KryptFS  fs;
  KryptNet net;

  bool boot() {
    hal.init();
    bool fsOk = fs.init();
    return fsOk;
  }

  String getVersion() {
    return KRYPT_VERSION;
  }
};

extern KryptKernel Krypt;

#endif
