#include "ThermalDetonator.h"

void ThermalDetonator::init() {
  Lights.init();
  Lights.on(0b1000, 200);

#ifdef USE_WIRELESS
  Wireless.init();
  Lights.on(0b0100, 200);
#endif

#ifdef SD_AUDIO
  Sound.init();
  Lights.on(0b0010, 200);
#endif

  state = TD_IDLE;

  // Button Config
  Lever = OneButton(MASTER_SW_PIN, HIGH, true);
  Enable = OneButton(ENABLE_SW_PIN, HIGH, true);

  // Lever Actions
  Lever.attachPressStart([]() {
    // Open Lever
    TD.doLeverOpen();
  });

  Lever.attachLongPressStop([]() {
    // Close Lever
    TD.doLeverClose();
  });

  // Enable Actions
  Enable.attachClick([]() {
    TD.doSinglePress();
  });

  Enable.attachLongPressStart([]() {
    TD.doLongPress();
  });

  Enable.attachDoubleClick([]() {
    TD.doDoublePress();
  });
}

void ThermalDetonator::tick() {
  Enable.tick();
  Lever.tick();
  Lights.tick();

#ifdef SD_AUDIO
  Sound.tick();
#endif

#ifdef USE_WIRELESS
  Wireless.tick();

  if (Wireless.available()) {
    handleWireless(Wireless.getData());
  }
#endif

  switch (state) {
#ifdef DEBUG
    case TD_IDLE:
      Lights.blinkSwitch(5000, 1000);
      break;
#endif

    case TD_STARTUP:
      Lights.blinkSwitch(1000, 200);
      break;

    case TD_LOOP:
      Lights.blinkSwitch(250, 20);
      Lights.nextSequence(1000);
      break;
  }
}

/****
 * State Machine Events
 */

void ThermalDetonator::doLeverOpen() {
  switch (state) {
    case TD_IDLE:
      goStartup();
      break;
  }
}

void ThermalDetonator::doLeverClose() {
  switch (state) {
    default:
      goIdle();
      break;
  }
}

void ThermalDetonator::doSinglePress() {
  switch (state) {
    case TD_STARTUP:
      goArm();
      break;
  }
}

void ThermalDetonator::doDoublePress() {
  switch (state) {
  }
}

void ThermalDetonator::doLongPress() {
  switch (state) {
    case TD_LOOP:
      goStartup();
      break;
  }
}


/****
 * State Machine Transitions
 */

void ThermalDetonator::nextState() {
  switch(state) {
    case TD_IDLE:
      goStartup();
      break;
    case TD_STARTUP:
      goArm();
      break;
  }
}

void ThermalDetonator::goStartup() {
  state = TD_STARTUP;
  Lights.off();
#ifdef SD_AUDIO
  Sound.playStartup();
#endif
}

void ThermalDetonator::goArm() {
  state = TD_LOOP;
  Lights.off();
#ifdef SD_AUDIO
  Sound.playLoop();
#endif
}

void ThermalDetonator::goIdle() {
  state = TD_IDLE;
  Lights.off();
#ifdef SD_AUDIO
  Sound.playShutdown();
#endif
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

#ifdef USE_WIRELESS
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
#endif

void ThermalDetonator::halt(uint8_t errorCode) {
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
