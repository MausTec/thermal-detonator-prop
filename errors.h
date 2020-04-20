#ifndef THERMALDETONATOR_ERRORS_H
#define THERMALDETONATOR_ERRORS_H

/**
 * Error Codes
 */
// No SD card detected or bad wiring.
#define ERR_NO_SD        0b0001
// Missing `start.wav`
#define ERR_NO_START_WAV 0b0011
// Missing `loop.wav`
#define ERR_NO_LOOP_WAV  0b0101
// Missing `shutdown.wav`
#define ERR_NO_SHUT_WAV  0b1001

#endif //THERMALDETONATOR_ERRORS_H
