#include <Arduino.h>

// INPUTS (FS-R6B)
#define TURN_CH1 6   // Receiver CH1
#define FWD_CH2 7    // Receiver CH2

// OUTPUTS (TB6612FNG)
#define R_DIR A1
#define R_PWM 3  
#define L_DIR A4
#define L_PWM 5 

#define OFFSET 1500
#define DEADZONE 55
#define MAX_PULSE 500

void setup() {
  // Set pin modes for inputs
  pinMode(TURN_CH1, INPUT);
  pinMode(FWD_CH2, INPUT);

  // Set pin modes for outputs
  pinMode(R_DIR, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
}

/**
 * Sets motor direction and speed based on input speed value.
 * @param dir_pin The direction control pin for the motor.
 * @param pwm_pin The PWM control pin for the motor.
 * @param speed The desired speed of the motor (-MAX_PULSE to MAX_PULSE).
 */
void setMotor(int dir_pin, int pwm_pin, int speed) {
  if (speed > 0) {
    digitalWrite(dir_pin, HIGH); // Set direction forward
  } else if (speed < 0) {
    digitalWrite(dir_pin, LOW); // Set direction backward
  } else {
    analogWrite(pwm_pin, 0); // Stop motor
    return;
  }

  // Map pulse to motor speed (0 to MAX_PULSE) to (0 to 255)
  int pwm_value = map(abs(speed), 0, MAX_PULSE, 0, 255);
  analogWrite(pwm_pin, pwm_value); // Set motor speed
}

void loop() {
  int fwd_value = pulseIn(FWD_CH2, HIGH, 50000) - OFFSET; // Read forward channel pulse width and apply offset
  int turn_value = pulseIn(TURN_CH1, HIGH, 50000) - OFFSET; // Read turn channel pulse width and apply offset

  // Apply deadzone
  if (abs(fwd_value) < DEADZONE) {
    fwd_value = 0;
  }
  if (abs(turn_value) < DEADZONE) {
    turn_value = 0;
  }

  int left_speed = fwd_value + turn_value; // Calculate left motor speed
  int right_speed = fwd_value - turn_value; // Calculate right motor speed

  // Set speeds for both motors
  setMotor(L_DIR, L_PWM, left_speed);
  setMotor(R_DIR, R_PWM, -right_speed);
}