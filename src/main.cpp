#include <Arduino.h>

const byte MOTOR_LEFT_SPEED = 9;
const byte MOTOR_LEFT_FORWARD = 7;
const byte MOTOR_LEFT_BACKWARD = 4;
const byte MOTOR_RIGHT_SPEED = 10;
const byte MOTOR_RIGHT_FORWARD = 12;
const byte MOTOR_RIGHT_BACKWORD = 8;

const int IR_SENSOR_FRONT_LEFT = A0;


void setup() {
  Serial.begin(115200);
  pinMode(MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_SPEED, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWORD, OUTPUT);
  pinMode(IR_SENSOR_FRONT_LEFT, INPUT);
}

void loop() {
  const int val= analogRead(A0);
  Serial.println(val);

  delay(100);
/*
  // Vorwärts
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWORD, LOW);

  analogWrite(MOTOR_LEFT_SPEED, 170);
  analogWrite(MOTOR_RIGHT_SPEED, 170);

  delay(1000);

  // Stop
  analogWrite(MOTOR_LEFT_SPEED, 0);
  analogWrite(MOTOR_RIGHT_SPEED, 0);

  delay(500);

  // Rückwärts
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWORD, HIGH);

  analogWrite(MOTOR_LEFT_SPEED, 170);
  analogWrite(MOTOR_RIGHT_SPEED, 170);

  delay(1000);

  // Stop
  analogWrite(MOTOR_LEFT_SPEED, 0);
  analogWrite(MOTOR_RIGHT_SPEED, 0);

  delay(500);
  */
}