#include <EEPROM.h> // to record steering input data and measure against position where the motor should be

#define POT_VCC 0
#define POT_GND 0

// CAR SENSOR INPUT PINS
#define POS_PIN A0 // mapped to wiper pin of potentiometer reporting actuator position
#define STEER_PIN A1 // voltage between 0.5 - 4.5V
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

#define CENTER 954 // when wheels are straight

#define DUTY 60 // DUTY cycle for the PWM PIN controlling the actuator set to 60 to move it slowly to prevent breaking anything

int reading = 0;
int steerIn = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(DIRN, OUTPUT);
  //digitalWrite(DIRN, HIGH);
  pinMode(PWM, OUTPUT);
  //analogWrite(PWM, 60);
  // set up actuator position reporting:
  Serial.begin(BAUD);
  setAccelPos();
}

void loop() {
  // put your main code here, to run repeatedly:
  //setAccelMode();
  reading = analogRead(POS_PIN);
  setPos(935);
  steerIn = constrain(analogRead(STEER_PIN), LOWER, UPPER);
  uint8_t brakes = digitalRead(BRAKE_PIN);
  // record actuator position
  Serial.print("ACTUATOR: ");
  Serial.println(analogRead(POS_PIN));
}

void setAccelPos() {
  while (reading != CENTER) {
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
    while (reading != pos) {
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
