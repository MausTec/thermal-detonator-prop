#include "ThermalDetonator.h"

void ThermalDetonator::init() {
  Lights.init();
  Wireless.init();
  Sound.init();

  state = TD_IDLE;

  // Button Config
  Enable = OneButton(ENABLE_SW_PIN, HIGH, false);

  Enable.attachClick([]() {
    TD.nextState();
  });

  Enable.attachLongPressStart([]() {
    TD.goIdle();
  });

  Enable.attachDoubleClick([]() {
    TD.goEasterEgg();
  });

  // Master Switch
  pinMode(MASTER_SW_PIN, INPUT);
}

void ThermalDetonator::tick() {
  Enable.tick();
  Lights.tick();
  Sound.tick();
  Wireless.tick();

  if (Wireless.available()) {
    handleWireless(Wireless.getData());
  }

  switch (state) {
    case TD_IDLE:
      Lights.blinkSwitch(5000, 1000);
      break;

    case TD_STARTUP:
      Lights.blinkSwitch(1000, 200);
      break;

    case TD_LOOP:
      Lights.blinkSwitch(250, 20);
      Lights.nextSequence(1000);
      break;
  }
}

void ThermalDetonator::nextState() {
  if (state == TD_IDLE) {
    goStartup();
  } else {
    goArm();
  }
}

void ThermalDetonator::goStartup() {
  state = TD_STARTUP;
  Sound.playStartup();
}

void ThermalDetonator::goArm() {
  state = TD_LOOP;
  Sound.playLoop();
}

void ThermalDetonator::goIdle() {
  state = TD_IDLE;
  Lights.off();
  Sound.playShutdown();
}

void ThermalDetonator::goEasterEgg() {
  if (state == TD_IDLE) {
    Sound.playLaundry();
  } else {
    state = TD_IDLE;
    Lights.off();
    Sound.playMoira();
  }
}

//== PRIVATE

void ThermalDetonator::handleWireless(uint8_t data) {
  Serial.print("A");
  Serial.println(Wireless.getAddress(), HEX);
  Serial.print("D");
  Serial.print(data, HEX);

  switch (data) {
    case 0:
      // reset condition, don't do anything.
      break;
    case 1:
      nextState();
      break;
    case 4:
      goIdle();
      break;
  }
}

void ThermalDetonator::halt(uint8_t errorCode) {
  return;
  while(true) {
    digitalWrite(LUMEN_1_PIN, (errorCode >> 3) & 1);
    digitalWrite(LUMEN_2_PIN, (errorCode >> 2) & 1);
    digitalWrite(LUMEN_3_PIN, (errorCode >> 1) & 1);
    digitalWrite(ENABLE_LUMEN_PIN, (errorCode >> 0) & 1);
    delay(300);
    digitalWrite(LUMEN_1_PIN, LOW);
    digitalWrite(LUMEN_2_PIN, LOW);
    digitalWrite(LUMEN_3_PIN, LOW);
    digitalWrite(ENABLE_LUMEN_PIN, LOW);
    delay(100);
  }
}

ThermalDetonator TD = ThermalDetonator();
