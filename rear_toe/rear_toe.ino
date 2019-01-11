#include <EEPROM.h> // to record steering input data and measure against position where the motor should be

#define POT_VCC A5
#define POT_GND A1

// CAR SENSOR INPUT PINS
#define POS_PIN A0 // mapped to wiper pin of potentiometer reporting actuator position
#define STEER_PIN A3 // voltage between 0.5 - 4.5V
#define BRAKE_PIN 12

#define DIRN 8
#define PWM 10


// since steering input is given from 0.5V to 4.5V we must set bounds and constrain
#define UPPER 921 // approx 4.5V from ADC reading
#define LOWER 102 // approx 0.5V ADC reading

#define BAUD 9600

// LIMIT ACTUATOR READINGS (MAX POSITION OF ACTUATOR IN EITHER DIRN)
#define AC_UPPER 964
#define AC_LOWER 927

// POSITION MAXIMUMS of +- 3 degrees as per league regulations
#define REG_UPPER 960
#define REG_LOWER 945 

#define CENTER 955 // when wheels are straight
#define DUTY 255 // DUTY cycle for the PWM PIN controlling the actuator set to 60 to move it slowly to prevent breaking anything
#define TOLERANCE 1 // error in the actuator reading that will be excepted ex pos should be 935 stop at reading of 936 if tolerance is 1
/*
  DRIVE_MODE
  0 : Accel Mode (Wheels stay straight)
  1 : Skid Pad (Wheels Turn Opposite to steering input)
  2 : Autocross / Endurance (Speed Influences Wheel behaviour)
*/
#define DRIVE_MODE 1

int reading = 0;
int steerIn = 0;

struct dataStore {
  int steerPos;
  int actuatorPos; // actual position of actuator
  int intendedPos; // where actuator should be for that steering value
}; // will use this data to measure the amount of error in the system.
// a potentiometer will act as a steering wheel.

void setup() {
  // put your setup code here, to run once:
  pinMode(DIRN, OUTPUT);
  pinMode(PWM, OUTPUT);
  //analogWrite(PWM, 60);
  // Set up steering pot
  pinMode(A5, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(POT_VCC, HIGH);
  digitalWrite(POT_GND, LOW);
  // set up actuator position reporting:
  Serial.begin(BAUD);
  setAccelPos(); // re orient to center
}

void loop() {
  // put your main code here, to run repeatedly:
  //setAccelMode();
  reading = analogRead(POS_PIN);
  steerIn = constrain(analogRead(STEER_PIN), LOWER, UPPER);
  uint8_t brakes = digitalRead(BRAKE_PIN);

  // Update Rear Wheels
  if (DRIVE_MODE == 0) {
    setPos(CENTER);
  } else if (DRIVE_MODE == 1) {
    int actPos = map(steerIn, LOWER, UPPER, REG_LOWER, REG_UPPER);
    setPos(actPos);
  }
  
  // record actuator position
  Serial.print("ACTUATOR: ");
  Serial.println(analogRead(POS_PIN));
  //Serial.println(steerIn);
}

void setAccelPos() {
  while (!approx(reading, CENTER, TOLERANCE)) {
    if (reading < CENTER) {
      digitalWrite(DIRN, HIGH);
    } else {
      digitalWrite(DIRN, LOW);
    }
    analogWrite(PWM, 60);
    reading = analogRead(POS_PIN);
    Serial.println(reading);
  }
  analogWrite(PWM, 0);
}

void setPos(int pos) { // general function called in a loop to set actuator to desired position
    while (!approx(reading, pos, TOLERANCE)) {
    if (reading < pos) {
      digitalWrite(DIRN, HIGH);
    } else {
      digitalWrite(DIRN, LOW);
    }
    analogWrite(PWM, 60);
    reading = analogRead(POS_PIN);
    Serial.println(reading);
  }
  analogWrite(PWM, 0);
}

uint8_t approx(int val, int target, int error) { // returns true if val is within the error range
  return abs(val - target) <= error;
}
