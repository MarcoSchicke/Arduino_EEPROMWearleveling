#include "EepromWearLeveling.h"

/**
  @brief  Initialize the object. Should be called before using another function.


           Data Type     | Size in Bytes | Range
           ------------- | ------------- | -------------------------------
           uint8_t       | 1             | 0..255
           uint16_t      | 2             | 0..65,535
           uint32_t      | 4             | 0..4,294,967,295
           uint64_t      | 8             | 0..18,446,744,073,709,551,615
           unsigned char | 1             | 0..255
           unsigned int  | 2             | 0..65535
           unsigned long | 4             | 0..4,294,967,295


  @param  _minimumAddress
           Start Address for wear leveling.
  @param  _maximumAddress
           End Address for wear leveling.
           Must not exceed the range of the EEPROM.
  @param  _valueSize
           Size of the Value to write in bytes.
  @param  _maximumValue
           Sets the maximum number which will be written to the EEPROM.
           If a higher number is inserted, the range of the EEPROM will be reset
           and an internal counter is set to sero as well.
*/
void EepromWearLeveling::initialize(uint16_t _minimumAddress = 0, uint16_t _maximumAddress = EEPROM.length() - 1, uint8_t _valueSize = 4, unsigned long _maximumValue = -2) {
  
  minimumAddress = _minimumAddress;
  lastWrittenAddress = _minimumAddress;
  valueSize = _valueSize;
  maximumValue = _maximumValue;
  

#ifdef EepromWearLevelingDebugInfo
  Serial.print("Minimum Address\t");
  Serial.println(minimumAddress);
  Serial.print("Maximum Address\t");
  Serial.println(_maximumAddress);
  Serial.print("Value Size\t");
  Serial.println(valueSize);
#endif



  if (_maximumAddress < EEPROM.length()) {
    maximumAddress = _maximumAddress;
  } else {
    maximumAddress = EEPROM.length() - 1;
#ifdef EepromWearLevelingDebugError
    Serial.println("!!! Initialization Values not correct !!!");
#endif
  }
}

/**
  @brief  Writes an unsigned long to the EEPROM.

  @param  _writeValue
           Value to write to the EEPROM
*/
void EepromWearLeveling::writeValue(unsigned long _writeValue) {

  // Check if Value has to be written
  if (highestWrittenUnsignedLong < _writeValue) {
    highestWrittenUnsignedLong = _writeValue;

    // check if counter has to be reset
    if (_writeValue > maximumValue) {
      clearRange();
    } else {

      // Check if address has to be reset to avoid overflow
      if ((lastWrittenAddress + (uint16_t)valueSize) > maximumAddress) {
        lastWrittenAddress = minimumAddress;
      } else {
        lastWrittenAddress += (uint16_t)valueSize;
      }
      // Write EEPROM
      // Source https://roboticsbackend.com/arduino-store-int-into-eeprom/
      EEPROM.update(lastWrittenAddress, (_writeValue >> 24) & 0xFF);
      EEPROM.update(lastWrittenAddress + 1, (_writeValue >> 16) & 0xFF);
      EEPROM.update(lastWrittenAddress + 2, (_writeValue >> 8) & 0xFF);
      EEPROM.update(lastWrittenAddress + 3, _writeValue & 0xFF);

#ifdef EepromWearLevelingDebugInfo
      Serial.print("Write value: ");
      Serial.print(_writeValue);
      Serial.print("\tHex ");
      Serial.print(_writeValue, HEX);
      Serial.print(" \tto address: ");
      Serial.println(lastWrittenAddress);
#endif
    }
  } else {
#ifdef EepromWearLevelingDebugInfo
    Serial.print("Value:\t");
    Serial.print(_writeValue);
    Serial.print("\t");
    Serial.print(_writeValue, HEX);
    Serial.println(" not written, to small");
#endif

  }
}

/**
  @brief  Returns the highest value in EEPROM in the specified range.

         This function should be executed once before the first value is written to get the latest location

  @return  Highest Value in EEPROM in the Range
*/
unsigned long EepromWearLeveling::getHighestUnsignedLong() {

  if (!highestWrittenUnsignedLong) {

#ifdef EepromWearLevelingDebugInfo
    Serial.println("Searching EEPROM for highest value");
#endif

    unsigned long tempHighestValue = 0;

    for (uint16_t i = minimumAddress ; i <= (maximumAddress - (uint16_t) valueSize); i += (uint16_t) valueSize) {
      tempHighestValue = readUnsignedLongFromEEPROM(i);
      if (tempHighestValue > highestWrittenUnsignedLong) {
        highestWrittenUnsignedLong = tempHighestValue;
        lastWrittenAddress = i;
#ifdef EepromWearLevelingDebugInfo
        Serial.print("Temporary highest value: ");
        Serial.print(highestWrittenUnsignedLong);
        Serial.print("  ");
        Serial.print(highestWrittenUnsignedLong, HEX);
        Serial.print(" found at address ");
        Serial.println(lastWrittenAddress);
#endif
      }
    }
  }

  return highestWrittenUnsignedLong;
}

/**
  @brief  Clears the range of the EEPROM.

*/
void EepromWearLeveling::clearRange() {
  for (uint16_t i = minimumAddress ; i <= maximumAddress ; i++) {
    EEPROM.update(i, 0);
    //Serial.print("Delete Address:\t");
    //Serial.println(i);
  }
  highestWrittenUnsignedLong = 0;
}

/**
  @brief  Clears all Data in the EEPROM.
  
  @details Note that other instances of this object might 
           have the highestWrittenUnsignedLong in the SRAM and 
		   will not be notified about deleting the EEPROM.

*/
void EepromWearLeveling::clearWholeEeprom() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.update(i, 0);
  }
  highestWrittenUnsignedLong = 0;
}
/**
  @brief  Reads an unsigned long.

  @param  address Start Adress for the data of the unsigned long.
  @return Unsigned long at the address.
*/
unsigned long EepromWearLeveling::readUnsignedLongFromEEPROM(uint16_t address)
{
  return ((unsigned long)EEPROM.read(address) << 24) +
         ((unsigned long)EEPROM.read(address + 1) << 16) +
         ((unsigned long)EEPROM.read(address + 2) << 8) +
         (unsigned long)EEPROM.read(address + 3);
}
