#include "ThermalWireless.h"

uint8_t ThermalWireless::listenAddress = 0;
volatile uint16_t ThermalWireless::data = 0;
volatile uint8_t ThermalWireless::dataIndex = 255;
volatile bool ThermalWireless::dataReady = false;
volatile uint8_t ThermalWireless::lastAddress = 0;
volatile uint8_t ThermalWireless::lastData = 0;

void ThermalWireless::init(uint8_t address = 0) {
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  listenAddress = address;
  attachInterrupt(digitalPinToInterrupt(RX_PIN), recvPulse, CHANGE);
}

void ThermalWireless::tick() {
  // noop
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
        uint8_t address = (data & 0xFF00) >> 8;
        uint8_t localData = (data & 0x00F0) >> 4;
        dataIndex = 255;

        // Store the last read:
        if (listenAddress == 0 || listenAddress == address) {
          // Discard duplicate packets.
          if (lastData != localData) {
            lastAddress = address;
            lastData = localData;
            dataReady = true;
            Serial.print(localData, HEX);
          }
        }
      }
    }
  }

  lastPulse = micros();
}