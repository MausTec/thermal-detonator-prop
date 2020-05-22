#include "../include/ThermalSystem.h"

#include "../config.h"

#define BAT_MIN 3.4
#define BAT_MAX 4.2

byte ThermalSystem::batteryLife() {
#ifdef VBAT_SENSE_PIN
  int value = analogRead(VBAT_SENSE_PIN);
  float voltage = value * 5.0 / 1023;

  float result = (
      (voltage - BAT_MIN) * (100 - 0) / (BAT_MAX - BAT_MIN) + 0
  );

#ifdef USE_SERIAL
  Serial.print("Battery = ");
  Serial.print(voltage);
  Serial.print("v (");
  Serial.print(result);
  Serial.println("%)");

  if (voltage > BAT_MAX) {
    Serial.println("  (Charging?)");
  }
#endif

  // Here we don't want to go below 0%
  result = max(result, 0);
  result = min(result, 255);

  return floor(result);
#else
  return 0;
#endif
}

ThermalSystem System = ThermalSystem();