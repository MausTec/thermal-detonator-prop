#ifndef __THERMAL_WIRELESS_h
#define __THERMAL_WIRELESS_h

#include <Arduino.h>

#include "config.h"

#define PAYLOAD_LENGTH 12

/**
 * This implements a bit-banged version of the HT12D decoder, and stores
 * data if it is not a duplicate packet. You can optionally set an address
 * to listen to (see @init).
 */
class ThermalWireless {
public:
  /**
   * Configure the wireless link, optionally listening and transmitting
   * data for a particular address. An address of 0 is a global broadcast,
   * which will almost certainly interfere with everyone's props and cause
   * what can generally be described as "a bad day".
   */
  void init(uint8_t address = 0);

  void tick();

  /**
   * Returns true if data is available to be read. This is cleared by calling
   * any of the get methods.
   * @return
   */
  bool available();

  /**
   * Returns the data in the buffer and resets available().
   * @return
   */
  uint8_t getData();

  /**
   * Returns the address in the buffer and resets available().
   * @return
   */
  uint8_t getAddress();

  /**
   * Assigns both the address and data in the buffer to the provided
   * variables, and resets available().
   * @param address
   * @param data
   */
  void get(uint8_t &address, uint8_t &data);

private:
  static uint8_t listenAddress;
  volatile static uint16_t data;
  volatile static uint8_t dataIndex;
  volatile static bool dataReady;
  volatile static uint8_t lastAddress;
  volatile static uint8_t lastData;

  static void recvPulse();

};


#endif
