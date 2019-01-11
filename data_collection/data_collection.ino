#include <EEPROM.h>

#define TOTAL_ENTRIES 170

struct dataStore {
  int steerPos;
  int actuatorPos; // actual position of actuator
  int intendedPos; // where actuator should be for that steering value
}; // will use this data to measure the amount of error in the system.
// a potentiometer will act as a steering wheel.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < TOTAL_ENTRIES; i++) {
    dataStore readIn;
    int addr = i * sizeof(dataStore);
    EEPROM.get(addr, readIn);
    // Format into text which can be copied into seperate file and parsed for analysis
    Serial.print(readIn.steerPos);
    Serial.print(",");
    Serial.print(readIn.actuatorPos);
    Serial.print(",");
    Serial.print(readIn.intendedPos);
    Serial.println("");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
