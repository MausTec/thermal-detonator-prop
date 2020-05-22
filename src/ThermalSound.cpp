#include "../config.h"

#ifdef SD_AUDIO
#include "../include/ThermalSound.h"
#include "../include/ThermalDetonator.h"

void ThermalSound::init() {
  pinMode(AUDIO_OUT_PIN, OUTPUT);

  pinMode(AMP_EN_PIN, OUTPUT);
  digitalWrite(AMP_EN_PIN, LOW);

  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);

#ifdef SD_AUDIO
  if (! SD.begin(SD_CS_PIN)) {
    ThermalDetonator::halt(ERR_NO_SD);
  }

  Audio.speakerPin = AUDIO_OUT_PIN;
  Audio.quality(1);
  Audio.setVolume(volume);
  Audio.loop(0);

  // Check Files
  if (! SD.exists("TD/startup.wav"))
    ThermalDetonator::halt(ERR_NO_START_WAV);
  if (! SD.exists("TD/loop.wav"))
    ThermalDetonator::halt(ERR_NO_LOOP_WAV);
  if (! SD.exists("TD/shutdown.wav"))
    ThermalDetonator::halt(ERR_NO_SHUT_WAV);
#endif
}

byte ThermalSound::stepVolumeDown() {
  volume -= 3;

  // Don't exceed maximum volume:
  if (volume > 7)
    volume = 7;

#ifdef USE_SERIAL
  Serial.print("Setting volume to ");
  Serial.println(volume);
#endif
  Audio.setVolume(volume);
  playBeep();
  return volume;
}

void ThermalSound::tick() {
  if (stopRequested && playing) {
    if (! Audio.isPlaying()) {
      Audio.disable();
      digitalWrite(AMP_EN_PIN, LOW);
      stopRequested = false;
      playing = false;
    }
  }
}

void ThermalSound::stop() {
  stopRequested = true;
  digitalWrite(AMP_EN_PIN, LOW);
  Audio.stopPlayback();
  Audio.disable();
}

void ThermalSound::playStartup() {
  loop(false);
  Audio.play("TD/startup.wav");
}

void ThermalSound::playBeep() {
  loop(false);
  Audio.play("Bonus/beep.wav");
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

  digitalWrite(AMP_EN_PIN, HIGH);
  delay(100); // Give the amp time to warm up
  playing = true;
}
#endif