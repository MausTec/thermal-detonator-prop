#include "../include/ThermalDetonator.h"
#include "../include/ThermalSystem.h"

void ThermalDetonator::init() {
  Lights.init();

#ifdef USE_WIRELESS
  Wireless.init();
  Lights.on(0b0100, 200);
#endif

#ifdef SD_AUDIO
  Sound.init();
  Lights.on(0b0010, 200);
#endif

  // Button Config
#ifdef MASTER_SW_PIN
  Lever = OneButton(MASTER_SW_PIN, HIGH, true);

  // Lever Actions
  Lever.attachPressStart([]() {
    // Open Lever
    TD.doLeverOpen();
  });

  Lever.attachLongPressStop([]() {
    // Close Lever
    TD.doLeverClose();
  });

  state = TD_IDLE;
#else
  state = TD_STARTUP;
#endif

  Enable = OneButton(ENABLE_SW_PIN, HIGH, true);

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

#ifdef SD_AUDIO
  Sound.playBeep();
#endif
}

void ThermalDetonator::tick() {
  Enable.tick();
#ifdef MASTER_SW_PIN
  Lever.tick();
#endif
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
#ifdef SD_AUDIO
    case TD_STARTUP:
      stepVolumeDown();
      break;
#endif

#ifndef ATTINY
    case TD_LOOP:
    case TD_IDLE:
      goEasterEgg();
      break;
#endif
  }
}

void ThermalDetonator::doLongPress() {
  switch (state) {
#ifndef ATTINY
    case TD_STARTUP:
      goBatteryLevel();
      break;
#endif

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

#ifndef ATTINY
void ThermalDetonator::goEasterEgg() {
  if (state == TD_IDLE) {
#ifdef SD_AUDIO
    Sound.playLaundry();
#endif
  } else {
    state = TD_IDLE;
    Lights.off();
#ifdef SD_AUDIO
    Sound.playMoira();
#endif
  }
}

void ThermalDetonator::goBatteryLevel() {
  byte level = System.batteryLife();

#ifdef USE_SERIAL
  Serial.print("Battery level: ");
  Serial.println(level);
#endif

  if (level >= 90) {
    Lights.on(0b1110, 1000);
  } else if (level >= 50) {
    Lights.on(0b1100, 1000);
  } else {
    Lights.on(0b1000, 1000);
  }
}
#endif

void ThermalDetonator::stepVolumeDown() {
#ifdef SD_AUDIO
  byte vol = Sound.stepVolumeDown();
  if (vol >= 4) {
    Lights.on(0b1110, 500);
  } else if (vol >= 2) {
    Lights.on(0b1100, 500);
  } else {
    Lights.on(0b1000, 500);
  }
#endif
}

//== PRIVATE

#ifdef USE_WIRELESS
void ThermalDetonator::handleWireless(uint8_t data) {
#ifdef USE_SERIAL
  Serial.print("A");
  Serial.println(Wireless.getAddress(), HEX);
  Serial.print("D");
  Serial.print(data, HEX);
#endif

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
