#include <Arduino.h>

#define LAT_CH2 5
#define TURN_CH1 6
#define R_DIR 7
#define R_PWM 8
#define L_DIR 9
#define L_PWM 10

#define OFFSET 1500
#define DEADZONE 20
#define MAX_PULSE 500
#define MAX_PWM 255

void setup() {
  // Flysky FS-i6X transmitter outputs a PWM signal on each channel, so I set those pins as inputs
  pinMode(LAT_CH2, INPUT);
  pinMode(TURN_CH1, INPUT);
  // Using a TB6612FNG motor driver, which has a direction pin and a PWM pin for each motor, so I set those as outputs
  pinMode(R_DIR, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
}

void loop() {
  // 1500us is the duration of the pulse if the stick is in the center, this allows me to map the 1000us-2000us to a -500us-500us
  int CH2Value = constrain(pulseIn(LAT_CH2, HIGH) - OFFSET, -MAX_PULSE, MAX_PULSE);
  int CH1Value = constrain(pulseIn(TURN_CH1, HIGH) - OFFSET, -MAX_PULSE, MAX_PULSE);

  // Apply deadzone
  if (abs(CH2Value) < DEADZONE) CH2Value = 0;
  if (abs(CH1Value) < DEADZONE) CH1Value = 0;

  int LAT_PWM = abs(CH2Value) * MAX_PWM / MAX_PULSE;
  int TURN_PWM = abs(CH1Value) * MAX_PWM / MAX_PULSE;

  // Set the PWM values to the motor drivers
  analogWrite(R_PWM, TURN_PWM);
  analogWrite(L_PWM, LAT_PWM);

  // Set the direction of the motors
  if (CH1Value > 0) {
    digitalWrite(R_DIR, HIGH);
  } else {
    digitalWrite(R_DIR, LOW);
  }

  if (CH2Value > 0) {
    digitalWrite(L_DIR, HIGH);
  } else {
    digitalWrite(L_DIR, LOW);
  }
}