#ifndef __THERMAL_WIRELESS_h
#define __THERMAL_WIRELESS_h

#include <Arduino.h>

#include "config.h"

#define PAYLOAD_LENGTH 12

class ThermalWireless {
public:
  void init();
  void tick();

  bool available();
  uint8_t getData();
  uint8_t getAddress();
  void get(uint8_t &address, uint8_t &data);

private:
  volatile static uint16_t data;
  volatile static uint8_t dataIndex;
  volatile static bool dataReady;
  volatile static uint8_t lastAddress;
  volatile static uint8_t lastData;

  static void recvPulse();

};


#endif
