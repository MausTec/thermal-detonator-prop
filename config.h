#ifndef __CONFIG_h
#define __CONFIG_h

/**
 * MISC Settings
 */
#ifndef DEVICE_ADDRESS
// This is the address used for receiving commands
// over the HT12 protocol.
#define DEVICE_ADDRESS 0x00
#endif

/**
 * Pin Configuration
 */

// Status Pins
#define BLINK_PIN         A7  // PD0  2

// RX/TX Pins
#define RX_PIN            2  // PD2  4
#define TX_PIN            14 // PC0 23

// Audio Pins
#define SD_CS_PIN         4  // PD4  6
#define AUDIO_OUT_PIN     9  // PB1 15

// TD Pins
#define MASTER_SW_PIN     7  // PD7 13
#define ENABLE_SW_PIN     8  // PB0 14
#define ENABLE_LUMEN_PIN  5  // PD5 11
#define LUMEN_1_PIN       6  // PD6 12
#define LUMEN_2_PIN       3  // PD3  5
#define LUMEN_3_PIN       10 // PB2 16

// Sensor Pins
#define VBAT_SENSE_PIN    A1 // PC1 24
#define VCHG_SENSE_PIN    A2 // PC2 25

#endif
