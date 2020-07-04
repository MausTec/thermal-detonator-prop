#include "../include/ThermalLights.h"
#include "../include/ThermalDetonator.h"

void ThermalLights::init() {
  pinMode(ENABLE_LUMEN_PIN, OUTPUT);
  pinMode(LUMEN_1_PIN, OUTPUT);
  pinMode(LUMEN_2_PIN, OUTPUT);
  pinMode(LUMEN_3_PIN, OUTPUT);

#ifndef ATTINY
  digitalWrite(LUMEN_1_PIN, HIGH);
  digitalWrite(LUMEN_2_PIN, HIGH);
  digitalWrite(LUMEN_3_PIN, HIGH);
  delay(1000);
#endif

  off();
}

void ThermalLights::tick() {
  // noop
}

void ThermalLights::blinkSwitch(int delayLen = 1000, int fadeLen = 0) {
  static long lastBlink = millis();
  static long lastFade = lastBlink;
  static bool fadeIn = false;
  static uint8_t val = 0;

  if (fadeLen > 0) {
    uint8_t fadeInterval = 32;
    int fadeRate = fadeLen / fadeInterval; // 31.25

    if (millis() - lastFade > fadeRate) {
      lastFade = millis();

      if (fadeIn) {
        if (val > 255 - fadeInterval) {
          val = 255;
        } else {
          val += fadeInterval;
        }
      } else {
        if (val < fadeInterval) {
          val = 0;
        } else {
          val -= fadeInterval;
        }
      }

      analogWrite(ENABLE_LUMEN_PIN, val);
    }

    // Swap fade direction:
    if (millis() - lastBlink > delayLen) {
      lastBlink = millis();
      fadeIn = !fadeIn;
    }
  } else {
    if (millis() - lastBlink > delayLen) {
      lastBlink = millis();

      val = (val == 0) ? 255 : 0;
      analogWrite(ENABLE_LUMEN_PIN, val);
    }
  }
}

void ThermalLights::nextSequence(int delayLen) {
  static long lastBlink = millis();
  static uint8_t patternIdx = 0;

  if (millis() - lastBlink > delayLen) {
    lastBlink = millis();

    uint8_t pattern = blinkenPattern[patternIdx/2];
    uint8_t shift = patternIdx % 2 ? 0 : 4;
    pattern >>= shift;

    digitalWrite(LUMEN_1_PIN, (pattern >> 2) & 1);
    digitalWrite(LUMEN_2_PIN, (pattern >> 1) & 1);
    digitalWrite(LUMEN_3_PIN, (pattern >> 0) & 1);
    patternIdx++;

    if (patternIdx >= 6) {
      patternIdx = 0;
    }
  }
}

void ThermalLights::off() {
  digitalWrite(ENABLE_LUMEN_PIN, LOW);
  digitalWrite(LUMEN_1_PIN, LOW);
  digitalWrite(LUMEN_2_PIN, LOW);
  digitalWrite(LUMEN_3_PIN, LOW);
}

void ThermalLights::on(uint8_t pattern, long delayMs) {
  digitalWrite(ENABLE_LUMEN_PIN, pattern & 0b0001);
  digitalWrite(LUMEN_1_PIN, pattern & 0b0010);
  digitalWrite(LUMEN_2_PIN, pattern & 0b0100);
  digitalWrite(LUMEN_3_PIN, pattern & 0b1000);

  if (delayMs > 0) {
    delay(delayMs);
    off();
  }
}