#include "../include/ThermalSystem.h"

#include "../config.h"

byte ThermalSystem::batteryLife() {
  int value = analogRead(VBAT_SENSE_PIN);
  float voltage = value * 5.0 / 1023;

  float result = (
      (voltage - 3.6) * (100 - 0) / (4.2 - 3.6) + 0
  );

  Serial.print("Battery = ");
  Serial.print(voltage);
  Serial.print("v (");
  Serial.print(result);
  Serial.println("%)");

  if (voltage > 4.4) {
    Serial.println("  (Charging?)");
  }

  return floor(result);
}

ThermalSystem System = ThermalSystem();