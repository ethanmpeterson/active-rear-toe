#include <EEPROM.h>

struct dataStore {
  int steerPos;
  int actuatorPos; // actual position of actuator
  int intendedPos; // where actuator should be for that steering value
}; // will use this data to measure the amount of error in the system.
// a potentiometer will act as a steering wheel.

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
