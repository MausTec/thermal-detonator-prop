#include "ThermalWireless.h"

volatile uint16_t ThermalWireless::data = 0;
volatile uint8_t ThermalWireless::dataIndex = 255;
volatile bool ThermalWireless::dataReady = false;
volatile uint8_t ThermalWireless::lastAddress = 0;
volatile uint8_t ThermalWireless::lastData = 0;

void ThermalWireless::init() {
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(RX_PIN), recvPulse, CHANGE);
}

void ThermalWireless::tick() {
  if (available()) {
    uint8_t address = 0;
    uint8_t data = 0;
    get(address, data);

    Serial.print("D");
    for (int i = 3; i >= 0; i--) {
      Serial.print((data >> i) & 1);
    }

    Serial.print("A");
    for (int i = 7; i >= 0; i--) {
      Serial.print((address >> i) & 1);
    }
  }
}

bool ThermalWireless::available() {
  return dataReady;
}

uint8_t ThermalWireless::getData() {
  dataReady = false;
  return lastData;
}

uint8_t ThermalWireless::getAddress() {
  dataReady = false;
  return lastAddress;
}

void ThermalWireless::get(uint8_t &address, uint8_t &data) {
  address = getAddress();
  data = getData();
  dataReady = false;
}

void ThermalWireless::recvPulse() {
  static long lastPulse = micros();

  bool high = digitalRead(RX_PIN);
  long duration = micros() - lastPulse;

  if (high) {
    // bit-width timeout, reset to beginning
    if (duration > 4000) {
      dataIndex = 255;
    }
  } else {
    // Capture bit-width on falling edge and push onto the data stack
    uint16_t bit = (duration < 500) ? 1 : 0;

    // Don't reset dataIndex until our start bit is found:
    if (dataIndex == 255) {
      if (bit == 0) {
        dataIndex = 0;
        data = 0;
      }
    } else {
      data |= bit << (15 - dataIndex);

      if (dataIndex++ >= PAYLOAD_LENGTH - 1) {
        dataIndex = 255;

        // Store the last read:
        dataReady = true;
        lastAddress = (data & 0xFF00) >> 8;
        lastData = (data & 0x00F0) >> 4;
      }
    }
  }

  lastPulse = micros();
}