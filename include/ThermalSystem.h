#ifndef THERMALDETONATOR_THERMALSYSTEM_H
#define THERMALDETONATOR_THERMALSYSTEM_H

#include <Arduino.h>

class ThermalSystem {
public:
  byte batteryLife();
};

extern ThermalSystem System;

#endif
