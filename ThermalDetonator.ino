/**
 * Thermal Detonator Prop Soundboard. See config.h for pinout configuration.
 * 
 * Features:
 * - Audio Playback via SD Module
 * - Three BlinkenLights
 * - One Pushingbutton
 * - One Fun Switch!
 * - Bonus BlinkenLight!
 * - Remote Control???
 * 
 * Wiring:
 * - Uhg
 */

#include <Arduino.h>

#include "config.h"
#include "ThermalDetonator.h"

void setup() {
  Serial.begin(115200);

  Serial.println("Hello, Sabine.");

  // Debug Pin (for now?).
  pinMode(BLINK_PIN, OUTPUT);

  TD.init();
}

void loop() {
  TD.tick();
}
