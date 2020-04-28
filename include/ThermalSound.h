#ifndef __THERMAL_SOUND_h
#define __THERMAL_SOUND_h

#include <Arduino.h>
#include <SD.h>
#include <TMRpcm.h>
#include <SPI.h>

class ThermalSound {
public:
  void init();

  /**
   * This should be called to ensure single-play audio stops when
   * it should.
   */
  void tick();

  /**
   * Stop audio playback, just in case those sick beats were too much
   * for you.
   */
  void stop();

  /**
   * Play TD/startup.wav 1 time
   */
  void playStartup();

  /**
   * Play TD/loop.wav on repeat
   */
  void playLoop();

  /**
   * Play TD/shutdown.wav 1 time
   */
  void playShutdown();

  /**
   * Play Bonus/moira.wav 1 time
   */
  void playMoira();

  /**
   * Play Bonus/laundry.wav 1 time
   */
  void playLaundry();

  /**
   * Plays a simple beep.
   */
 void playBeep();

  /**
   * Decrement volume in a cycle
   */

  byte stepVolumeDown();

private:
  bool stopRequested = false;
  bool playing = false;
  TMRpcm Audio;
  byte volume = 5;

  void loop(bool forever);
};


#endif
