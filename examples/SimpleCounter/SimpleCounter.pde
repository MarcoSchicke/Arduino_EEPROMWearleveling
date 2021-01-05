/********************************************************************************//**
   @file
   @brief   This file shows how to create a simple counter in the eeprom.

   @author  Marco Schicke
   @date    January 2021

   @version 00
    - initial release


**************************************************************************************/


#include <EepromWearLeveling.h>

/// Static Declaration of the object myCounter
EepromWearLeveling myCounter;


/**
  @brief Program starts here
*/
void setup()
{
  delay(600);
  Serial.begin(9600);
  Serial.println("\nEeprom Simple Counter\n");

  // Initialize the counter over the whole eeprom.
  //  (From Adress 0 to the full length)
  // 999 states the maximum number of the counter.
  myCounter.initialize(0, EEPROM.length() - 1, 4, 999);

}

/**
  @brief Called after setup.

  @details Increases the counter by 1 and writes its value to the serial port.
*/
void loop()
{

  // write Counter Value to Serial Port
  Serial.print("Counter Value: ");
  Serial.println(myCounter.getHighestUnsignedLong());

  // increase counter by one
  myCounter.writeValue(myCounter.getHighestUnsignedLong() + 1);

  // wait half a second
  delay(500);

}
