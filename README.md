# Arduino EEPROMWearleveling Library #
https://github.com/MarcoSchicke/Arduino_EEPROMWearleveling

This library allows to write a counter on different locations of an EEPROM. Every new value is written on a different location of the EEPROM which increases the livetime and reduces the wear out.


A section of the EEPROM needs to be addressed, default setting is the whole EEPROM. The bigger the range the lower is the wear.
Optional a maximum counter can be set to allow the counter to be reset at a certain number( e.g. 1000 instead of 1024).

By now only writing and reading unsigned long values are supported.

Examples are provided with the library.