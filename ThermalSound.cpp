#include "ThermalSound.h"
#include "ThermalDetonator.h"

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
#endif
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
  digitalWrite(AMP_EN_PIN, LOW);
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

  digitalWrite(AMP_EN_PIN, HIGH);
  playing = true;
}

//Underworld melody
int underworld_melody[] = {
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, 0,
    0,
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
    NOTE_AS3, NOTE_AS4, 0,
    0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, 0,
    0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
    NOTE_DS3, NOTE_DS4, 0,
    0,

    NOTE_DS4, NOTE_D4, NOTE_CS4,
    NOTE_C4, NOTE_DS4,
    NOTE_D4, NOTE_GS3,
    NOTE_G3, NOTE_CS4,
    NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
    NOTE_GS4, NOTE_DS4, NOTE_B3,
    NOTE_AS3, NOTE_A3, NOTE_GS3,
    0, 0, 0
};
//Underwolrd tempo
int underworld_tempo[] = {
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    3,
    12, 12, 12, 12,
    12, 12, 6,
    6, 18, 18, 18,
    6, 6,
    6, 6,
    6, 6,
    18, 18, 18, 18, 18, 18,
    10, 10, 10,
    10, 10, 10,
    3, 3, 3
};

void ThermalSound::playUnderworld() {
  playMidi(underworld_melody, underworld_tempo, sizeof(underworld_melody) / sizeof(int));
}

void ThermalSound::playMidi(int pitches[], int duration[], int size) {
  digitalWrite(AMP_EN_PIN, HIGH);
//  delay(100);

  for (int thisNote = 0; thisNote < size; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / duration[thisNote];
    buzz(pitches[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(0, noteDuration);
  }

  digitalWrite(AMP_EN_PIN, LOW);
}

void ThermalSound::buzz(long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  uint8_t lightPin;

  if (frequency % 3 == 0) {
    lightPin = LUMEN_3_PIN;
  } else if (frequency % 2 == 0) {
    lightPin = LUMEN_2_PIN;
  } else {
    lightPin = LUMEN_1_PIN;
  }

  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(AUDIO_OUT_PIN, HIGH); // write the buzzer pin high to push out the diaphram
    digitalWrite(lightPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(AUDIO_OUT_PIN, LOW); // write the buzzer pin low to pull back the diaphram
    digitalWrite(lightPin, LOW); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}