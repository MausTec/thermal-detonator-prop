#ifndef __CONFIG_h
#define __CONFIG_h

#include "errors.h"

/**
 * Define ATTiny here if compiling for a tiny.
 */
#define ATTINY

/**
 * Pin Configuration
 */

#ifndef ATTINY

  // Status Pins
  #define BLINK_PIN         A7  // PD0  2

  // RX/TX Pins
  #define RX_PIN            2  // PD2  4
  #define TX_PIN            A0 // PC0 23

  #define RF_CE_PIN         A0
  #define RF_CSN_PIN        A4
  #define RF_IRQ_PIN        2

  // Audio Pins
  #define SD_CS_PIN         4  // PD4  6
  #define AUDIO_OUT_PIN     9  // PB1 15
  #define AMP_EN_PIN        A3 // ADC3 26

  // Sensor Pins
  #define VBAT_SENSE_PIN    A1 // PC1 24
  #define VBUS_SENSE_PIN    A2 // PC2 25

  /**
   * MISC Settings
   */
  #ifndef DEVICE_ADDRESS
    // This is the address used for receiving commands
    // over the HT12 protocol.
    #define DEVICE_ADDRESS 0x00
  #endif

  /**
   * Define USE_SERIAL to enable Serial IO and debugging.
   */
  #define USE_SERIAL

  /**
   * Define SD_AUDIO to enable the SD card audio, otherwise
   * all you can play are chiptunes.
   */
  #define SD_AUDIO

  /**
   * Define USE_WIRELESS to enable the NRF24 / HT12 wireless
   * modules.
   */
  #define USE_WIRELESS

#endif

// TD Pins
#ifdef ATTINY
  #ifdef ATTINY_84
    #define MASTER_SW_PIN     10  // PD7 13
    #define ENABLE_SW_PIN     9  // PB0 14
    #define ENABLE_LUMEN_PIN  5  // PD5 11
    #define LUMEN_1_PIN       6  // PD3 05
    #define LUMEN_2_PIN       7  // PD6  12
    #define LUMEN_3_PIN       8 // PB2 16
  #else
    #define ENABLE_SW_PIN     3
    #define ENABLE_LUMEN_PIN  4
    #define LUMEN_1_PIN       1
    #define LUMEN_2_PIN       0
    #define LUMEN_3_PIN       2
  #endif
#else
  #define MASTER_SW_PIN     7  // PD7 13
  #define ENABLE_SW_PIN     8  // PB0 14
  #define ENABLE_LUMEN_PIN  5  // PD5 11
  #define LUMEN_1_PIN       3  // PD3 05
  #define LUMEN_2_PIN       6  // PD6  12
  #define LUMEN_3_PIN       10 // PB2 16
#endif

#endif
