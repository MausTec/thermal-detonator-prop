#ifndef __THERMAL_LIGHTS_h
#define __THERMAL_LIGHTS_h

#include <Arduino.h>

class ThermalLights {
public:
  void init();

  /**
   * Turns off all the lights.
   */
  void off();

  void on(uint8_t pattern, long delay);

  void tick();

  /**
   * Blinks, and optionally fades, the enable switch.
   *
   * @param delayLen
   * @param fadeLen
   */
  void blinkSwitch(int delayLen = 1000, int fadeLen = 0);

  /**
   * Steps into the next pattern on the front lights. The pattern
   * is defined as a sequence of bytes, read in order one half at
   * a time (upper 4-bits -> lower 4-bits). Of each nibble, the lower
   * 3 bits is used to determine the light sequence.
   *
   * @param delayLen
   */
  void nextSequence(int delayLen = 1000);

private:

  const uint8_t blinkenPattern[3] = {
      0b00010010,
      0b00110100,
      0b01010110
  };

};


#endif
