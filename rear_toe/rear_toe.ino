#include <EEPROM.h> // to record steering input data and measure against position where the motor should be

#define POT_VCC 0
#define POT_GND 0

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

// LIMIT ACTUATOR READINGS
#define AC_UPPER 964
#define AC_LOWER 927

#define CENTER 955 // when wheels are straight
#define DUTY 255 // DUTY cycle for the PWM PIN controlling the actuator set to 60 to move it slowly to prevent breaking anything
#define TOLERANCE 1 // error in the actuator reading that will be excepted ex pos should be 935 stop at reading of 936 if tolerance is 1
/*
  DRIVE_MODE
  0 : Accel Mode (Wheels stay straight)
  1 : Skid Pad (Wheels Turn Opposite to steering input)
  2 : Autocross / Endurance (Speed Influences Wheel behaviour)
*/
#define DRIVE_MODE 0

int reading = 0;
int steerIn = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(DIRN, OUTPUT);
  //digitalWrite(DIRN, HIGH);
  pinMode(PWM, OUTPUT);
  //analogWrite(PWM, 60);
  // Set up steering pot
  pinMode(A5, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A5, HIGH);
  digitalWrite(A1, LOW);
  // set up actuator position reporting:
  Serial.begin(BAUD);
  setAccelPos();
}

void loop() {
  // put your main code here, to run repeatedly:
  //setAccelMode();
  reading = analogRead(POS_PIN);
  steerIn = constrain(analogRead(STEER_PIN), LOWER, UPPER);
  uint8_t brakes = digitalRead(BRAKE_PIN);

  // Update Rear Wheels
  if (DRIVE_MODE == 0) {
    //setPos(CENTER);
  } else if (DRIVE_MODE == 1) {
    
  }
  
  // record actuator position
  //Serial.print("ACTUATOR: ");
  //Serial.println(analogRead(POS_PIN));
  Serial.println(steerIn);
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
