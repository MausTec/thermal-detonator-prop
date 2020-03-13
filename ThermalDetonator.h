#ifndef __THERMAL_DETONATOR_h
#define __THERMAL_DETONATOR_h

#include <Arduino.h>
#include <OneButton.h>

#include "config.h"

#include "ThermalLights.h"
#include "ThermalSound.h"
#include "ThermalWireless.h"

/**
 * State Constants
 */
#define TD_IDLE       0
#define TD_STARTUP    1
#define TD_LOOP       2
#define TD_SHUTDOWN   3

/**
 * Error Codes
 */
#define ERR_NO_SD        0b0001
#define ERR_NO_START_WAV 0b0011
#define ERR_NO_LOOP_WAV  0b0101
#define ERR_NO_SHUT_WAV  0b1001

/**
 * Core management class for the Thermal Detonator prop.
 * This class handles state information and transitions, but delegates other
 * control functions down to child classes.
 * 
 * See:
 * - ThermalLights
 * - ThermalSound
 * - ThermalWireless
 * 
 */
class ThermalDetonator {
public:
  void init();
  void tick();

  /**
   * Step through the lazy state machine, doing whatever is logically
   * the next correct thing to do.
   */
  void nextState();

  /**
   * Go forth unto thine startup sequence. Plays a sound and violently
   * flashes the enable button. This should happen when the lever opens.
   */
  void goStartup();

  /**
   * Arms the prop, playing the loop sound and stepping through the lumen
   * sequence. This should happen when the button is pushed.
   */
  void goArm();

  /**
   * Returns the prop to idle, playing the shutdown sound IF the prop was
   * previously armed.
   */
  void goIdle();

  /**
   * Plays an easter egg sound. If the prop was armed, this shuts it down
   * with a Moira voice line. If the prop was idle, this will drop some sick
   * beats, yo.
   */
  void goEasterEgg();

  /**
   * Halts program and outputs an error code on the Blinkenlights.
   * @param errorCode 4-bits only, plz.
   */
  static void halt(uint8_t errorCode);

private:
  uint8_t state = TD_IDLE;
  ThermalLights Lights = ThermalLights();
  ThermalSound Sound = ThermalSound();
  ThermalWireless Wireless = ThermalWireless();
  OneButton Enable;
};

extern ThermalDetonator TD;

#endif
