# Thermal Detonator!

## Error Codes

Error codes are indicated by the lights rapidly flashing the same pattern.
They are read as a binary 4-bit number, with the red button as the LSB, and the
3 LEDs across the front as the upper bits:

|Hex|Bin|Code|Description|
|---|---|---|---|
|`0x1`|`---R`|` `| |
|`0x2`|`--W-`|`ERR_NO_SD`|No SD card detected or bad wiring.|
|`0x3`|`--WR`|` `| |
|`0x4`|`-W--`|`ERR_NO_START_WAV`|Missing `start.wav`|
|`0x5`|`-W-R`|` `| |
|`0x6`|`-WW-`|`ERR_NO_LOOP_WAV`|Missing `loop.wav`|
|`0x7`|`-WWR`|` `| |
|`0x8`|`W---`|` `| |
|`0x9`|`W--R`|` `| |
|`0xA`|`W-W-`|`ERR_NO_SHUT_WAV`|Missing `shutdown.wav`|
|`0xB`|`W-WR`|` `| |
|`0xC`|`WW--`|` `| |
|`0xD`|`WW-R`|` `| |
|`0xE`|`WWW-`|` `| |
|`0xF`|`WWWR`|` `| |


