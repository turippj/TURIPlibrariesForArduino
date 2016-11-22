#ifndef TURIP_CLIENT_H
#define TURIP_CLIENT_H
#include <Arduino.h>

class cl_TURIPdevice{
public: // 公開関数
  cl_TURIPdevice(uint64_t id);
  cl_TURIPdevice();
  int (*read)(uint64_t, uint8_t, void*, size_t);
  int (*write)(uint64_t, uint8_t, void*, size_t);
public: // 公開変数
  uint64_t id;
};

class cl_TURIPperipheral{
public: // 公開関数
  cl_TURIPperipheral();
  int (*scan)();
public: // 公開変数
  cl_TURIPdevice** device;
  int numof_device;
};

class cl_TURIPclient{
public: // 公開関数
  cl_TURIPclient();
  int scan();
  int addPeripheral(cl_TURIPperipheral* peripheral);
private: // 非公開関数
  int addDevice(cl_TURIPdevice* device);
  // int resizeDeviceList(size_t numof_device); TODO: リストを可変配列化する
  // int resizePeripheralList(size_t numof_device); TODO: リストを可変配列化する
public:
  cl_TURIPdevice** device;
  cl_TURIPperipheral** peripheral;
  size_t numof_device, sizeof_deviceList;
  size_t numof_peripheral, sizeof_peripheralList;
};
extern cl_TURIPclient TURIPclient;

class TURIP{
public: // 公開関数
  TURIP(uint64_t id);
  TURIP();
  int begin(uint64_t id);
  int read(uint8_t port, void* data, size_t datasize);
  int write(uint8_t port, void* data, size_t datasize);
private: // 非公開変数
  uint64_t id;
  cl_TURIPdevice* device;
};

#endif
