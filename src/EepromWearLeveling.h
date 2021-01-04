#ifndef EepromWearLeveling_h
#define EepromWearLeveling_h

#include <Arduino.h>
#include <EEPROM.h>

// Prints debug comments to serial if enabled
// #define EepromWearLevelingDebugInfo

// Prints errors to serial if enabled
#define EepromWearLevelingDebugError


/********************************************************************************//**
   @class   EepromWearLeveling
   @brief   This class allows to write a count up on different locations of
            an EEPROM. Since an EEPROM can be written only a limited amount of time
			this class enhances the livetime of the arduino.

            A section of the EEPROM needs to be addressed. The bigger the range the
			lower is the wear.
			Optional a maximum counter can be set to allow the counter to be reset
            at a certain number( e.g. 1000 instead of 1024).

            Be aware that different models of the Arduino have different EEPROM sizes.
            
                 Model               | EEPROM storage
                 ------------------- | ---------------
                 Arduno Duemilanove  | 512b
                 Arduino Uno         | 1kb
                 Arduino Mega        | 4kb

   @todo    In this version only a 4 byte unsigned long can be written.
            For future versinos this can be changed to allow other numbers
			as well.

   @author  Marco Schicke
   @date    January 2021

   @version 02
    - improved comments

   @version 01
    - fixed variables not being static
    - fixed reading variables over range
    - improved test 
    - improved comments

   @version 00
    - initial release


**************************************************************************************/

class EepromWearLeveling {

  public:
    void initialize(uint16_t _minimumAddress = 0, 
	  uint16_t _maximumAddress = EEPROM.length() - 1, 
	  uint8_t _valueSize = 4, 
	  unsigned long _maximumValue = -2);
      void writeValue(unsigned long writeValue);
    unsigned long getHighestUnsignedLong();
    void clearRange();
    void clearWholeEeprom();


  protected:
    unsigned long readUnsignedLongFromEEPROM(uint16_t address);

    /// Start address for EEPROM wear leveling
    uint16_t minimumAddress = 0;

    /// Maximum Site for EEPROM wear leveling
    uint16_t maximumAddress = EEPROM.length() - 1;

    /// Last address which has the highest value
    uint16_t lastWrittenAddress = 0;

    /// Size of the value to store in byte
    uint8_t valueSize = 4;

    /// highest value before counter reset
    unsigned long maximumValue = 0 - 2;

    /// Last written value for faster processing
    unsigned long highestWrittenUnsignedLong = 0;

};
#endif
