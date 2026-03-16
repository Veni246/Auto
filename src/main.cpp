#include <Arduino.h>

const byte MOTOR_LEFT_SPEED = 9;
const byte MOTOR_LEFT_FORWARD = 4;
const byte MOTOR_LEFT_BACKWARD = 7;
const byte MOTOR_RIGHT_SPEED = 10;
const byte MOTOR_RIGHT_FORWARD = 8;
const byte MOTOR_RIGHT_BACKWORD = 12;

void setup() {
  pinMode(MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_SPEED, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWORD, OUTPUT);
}

void loop() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWORD, LOW);

  analogWrite(MOTOR_LEFT_SPEED, 255);
  analogWrite(MOTOR_RIGHT_SPEED, 255);

  delay(1000);

  // Stop
  analogWrite(MOTOR_LEFT_SPEED, 0);
  analogWrite(MOTOR_RIGHT_SPEED, 0);

  delay(200);

  // Rückwärts
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWORD, HIGH);

  analogWrite(MOTOR_LEFT_SPEED, 200);
  analogWrite(MOTOR_RIGHT_SPEED, 200);

  delay(1000);

  // Stop
  analogWrite(MOTOR_LEFT_SPEED, 0);
  analogWrite(MOTOR_RIGHT_SPEED, 0);

  delay(200);
  }