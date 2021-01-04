# ArduinoEEPROMWearleveling

This Library allows to write a count up on different locations of an EEPROM. Since an EEPROM can be written only a limited amount of time this class enhances the livetime of the arduino.

A section of the EEPROM needs to be addressed. The bigger the range the lower is the wear.
Optional a maximum counter can be set to allow the counter to be reset at a certain number( e.g. 1000 instead of 1024).
Be aware that different Arduino models have different EEPROM sizes.
