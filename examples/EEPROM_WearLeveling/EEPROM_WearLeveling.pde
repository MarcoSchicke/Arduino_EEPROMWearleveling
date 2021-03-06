/********************************************************************************//**
   @file    
   @brief   This file shows how to use the EEPROM Wear Leveling library.
            At the same time, this can also be used for testing purposes.

   @author  Marco Schicke
   @date    January 2021

  @version 01
    - comments improved

  @version 00
    - initial release


**************************************************************************************/

#include <EepromWearLeveling.h>

// Static Declaration
EepromWearLeveling MyData;
EepromWearLeveling MyData2;

/**
  @brief Program starts here
*/
void setup()
{
  delay(600);
  Serial.begin(9600);
  Serial.println("\nTesting Eeprom\n");

  // Send Information for easier debugging
  Serial.print("\n\nFile: ");
  Serial.println(__FILE__);
  Serial.print("Compile Date: ");
  Serial.print(__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);
  Serial.println("\n\n");


  // Delete the whole EEprom before the first run.
  // Comment this line first test.
  MyData.clearWholeEeprom();


  // Test 1 *********************************************************************
  Serial.println("\n \n Test 1:\n");

  Serial.println("Initialize");
  MyData.initialize(0, EEPROM.length() / 8, 4);
  Serial.println("Delete EEPROM Range");
  MyData.clearRange();
  // Write Value
  Serial.println("Write 100");
  MyData.writeValue(100);
  Serial.print("Value in EEPROM:");
  Serial.println(MyData.getHighestUnsignedLong());
  // Write Value
  Serial.println("Write 200");
  MyData.writeValue(200);
  Serial.print("Value in EEPROM:");
  Serial.println(MyData.getHighestUnsignedLong());

  // Will be ignorred since the Value is to low
  Serial.println("Write 180");
  MyData.writeValue(180);
  Serial.print("Value in EEPROM:");
  Serial.println(MyData.getHighestUnsignedLong());

  delay(5000);



  // Test 2 *********************************************************************
  // Small area used to test if written data stays in range.
  // Counter will be reset after exeeding 19.
  Serial.println("\n \n Test 2:\n");

  MyData2.initialize( (EEPROM.length() / 8) + 1, (EEPROM.length() / 8) + 15, 4, 19);
  //Delete for testing purposes
  MyData2.clearRange();

  for (int i = 0 ; i < 30; i ++) {
    MyData2.writeValue(MyData2.getHighestUnsignedLong() + 1L);
    delay(300);
    Serial.println(MyData2.getHighestUnsignedLong());
  }

  delay(5000);


  // Test 3 *********************************************************************

  //create and destroy a object many times
  Serial.println("\n \n Test 3:\n");

  while (true) {
    Serial.println("---\n");
    EepromWearLeveling MyData3;
    EepromWearLeveling MyData4;
    MyData3.initialize((EEPROM.length() / 2) + 1, EEPROM.length() - 1, 4);
    MyData4.initialize((EEPROM.length() / 8) + 16, (EEPROM.length() / 2), 4, 30);
    MyData3.writeValue(MyData3.getHighestUnsignedLong() + 1L);
    MyData3.writeValue(500L);
    Serial.println(MyData3.getHighestUnsignedLong());

    MyData4.writeValue(MyData4.getHighestUnsignedLong() + 1L);
    Serial.println(MyData4.getHighestUnsignedLong());

    delay(1000);
  }
}

/**
  @brief Called after setup.
*/
void loop()
{

}
