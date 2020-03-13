#include "ThermalSound.h"
#include "ThermalDetonator.h"

void ThermalSound::init() {
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);

  if (! SD.begin(SD_CS_PIN)) {
    ThermalDetonator::halt(ERR_NO_SD);
  }

  Audio.speakerPin = AUDIO_OUT_PIN;
  Audio.quality(0);
  Audio.setVolume(5);
  Audio.loop(0);

  // Check Files
  if (! SD.exists("TD/startup.wav"))
    ThermalDetonator::halt(ERR_NO_START_WAV);
  if (! SD.exists("TD/loop.wav"))
    ThermalDetonator::halt(ERR_NO_LOOP_WAV);
  if (! SD.exists("TD/shutdown.wav"))
    ThermalDetonator::halt(ERR_NO_SHUT_WAV);
}

void ThermalSound::tick() {
  if (stopRequested && playing) {
    if (! Audio.isPlaying()) {
      Audio.disable();
      stopRequested = false;
      playing = false;
    }
  }
}

void ThermalSound::stop() {
  stopRequested = true;
  Audio.stopPlayback();
  Audio.disable();
}

void ThermalSound::playStartup() {
  loop(false);
  Audio.play("TD/startup.wav");
}

void ThermalSound::playLoop() {
  loop(true);
  Audio.play("TD/loop.wav");
}

void ThermalSound::playShutdown() {
  loop(false);
  Audio.play("TD/shutdown.wav");
}

void ThermalSound::playMoira() {
  loop(false);
  Audio.play("Bonus/moira.wav");
}

void ThermalSound::playLaundry() {
  loop(false);
  Audio.play("Bonus/laundry.wav");
}

void ThermalSound::loop(bool forever) {
  if (forever) {
    stopRequested = false;
    Audio.loop(1);
  } else {
    stopRequested = true;
    Audio.loop(0);
  }

  playing = true;
}