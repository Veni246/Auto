/*
 * Klasse:   6AAELI
 * Datum:     20.04.2026
 */

// ===== INCLUDES =====
#include <Arduino.h>

void checkButtons();
void readSensors();
void processStateMachine();
void driveForward();
void driveBackward();
void forward();
void backward(int speed);
void stop();
void printStatus();


// ===== PIN-DEFINITIONEN =====
#define IR_FRONT         A0    
#define IR_RIGHT         A1  
#define IR_LEFT          A2 

#define START_BUTTON      3     
#define STOP_BUTTON       2    

#define MOTOR_R_SPEED     10
#define MOTOR_R_FWD       12
#define MOTOR_R_BWD       8

#define MOTOR_L_SPEED     9
#define MOTOR_L_FWD       7
#define MOTOR_L_BWD       4

// ===== IR-SENSOR KALIBRIERPARAMETER =====
// Formel: laenge [cm] = m' / (raw + d') - k  (aus Excelberechnung)
const float PARAM_K_FRONT   = 10;
const float PARAM_D_FRONT   = -0.0761905;
const float PARAM_M_FRONT   = 14625;

const float PARAM_K_RIGHT   = 5;
const float PARAM_D_RIGHT   = -0.1538462;
const float PARAM_M_RIGHT   = 7194.642857;

const float PARAM_K_LEFT    = 5;
const float PARAM_D_LEFT    = 1.076923077;
const float PARAM_M_LEFT    = 7012.5;

const uint16_t FRONT_STOP      = 40; 
const uint16_t FRONT_CLEAR     = 50;
const uint16_t TARGET_DIST     = 35;

const uint8_t BASE_SPEED = 120;
const uint8_t MAX_SPEED = 255;

/*
 * Zustandsübergänge:
 *   STOPP  --[Start-Button]--> FORWARD
 *   FORWARD --[ir_front < FRONT_STOP]--> BACKWARD
 *   FORWARD --[Stop-Button]--> STOPP
 *   BACKWARD --[ir_front >= FRONT_CLEAR]--> FORWARD
 *   BACKWARD --[Stop-Button]--> STOPP
 */
enum State { STOPP, FORWARD, BACKWARD };

// ===== GLOBALE VARIABLEN =====
State state = STOPP;

uint16_t ir_front = 70, ir_right = 35, ir_left = 35;
uint16_t ir_front_prev = 70, ir_right_prev = 35, ir_left_prev = 35;

int spd_left  = 0;   // aktuell gesetzte Motorgeschwindigkeit links
int spd_right = 0;   // aktuell gesetzte Motorgeschwindigkeit rechts

unsigned long previous_millis_20ms  = 0;
unsigned long previous_millis_100ms = 0;


// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(IR_FRONT, INPUT);
  pinMode(IR_RIGHT, INPUT);
  pinMode(IR_LEFT, INPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(STOP_BUTTON, INPUT_PULLUP);

  pinMode(MOTOR_R_SPEED, OUTPUT);
  pinMode(MOTOR_R_FWD, OUTPUT);
  pinMode(MOTOR_R_BWD, OUTPUT);
  pinMode(MOTOR_L_SPEED, OUTPUT);
  pinMode(MOTOR_L_FWD, OUTPUT);
  pinMode(MOTOR_L_BWD, OUTPUT);

  stop();
  Serial.println("Bereit. Start-Taste druecken.");
}


// ===== LOOP =====
void loop() {

  checkButtons();

  if (millis() - previous_millis_20ms >= 20) {
    previous_millis_20ms = millis();
    readSensors();
    processStateMachine();
  }

  if (millis() - previous_millis_100ms >= 100) {
    previous_millis_100ms = millis();
    printStatus();
  }
}


void readSensors() {
  uint16_t raw, new_val;

  ir_front_prev = ir_front;
  ir_right_prev = ir_right;
  ir_left_prev  = ir_left;

  raw     = analogRead(IR_FRONT);
  new_val = (uint16_t)(PARAM_M_FRONT / (raw + PARAM_D_FRONT)) - PARAM_K_FRONT;
  if(new_val > 150) new_val = 151;      // Maximalwert begrenzen
  else if(new_val < 20) new_val = 19;   // Minimalwert begrenzen

  ir_front = (new_val + ir_front_prev) / 2;  // Glättung

  raw     = analogRead(IR_RIGHT);
  new_val = (uint16_t)(PARAM_M_RIGHT / (raw + PARAM_D_RIGHT)) - PARAM_K_RIGHT;
  if(new_val > 80) new_val = 81;        // Maximalwert begrenzen
  else if(new_val < 10) new_val = 9;     // Minimalwert begrenzen
  
  ir_right = (new_val + ir_right_prev) / 2;
  
  raw     = analogRead(IR_LEFT);
  new_val = (uint16_t)(PARAM_M_LEFT / (raw + PARAM_D_LEFT)) - PARAM_K_LEFT;
  if(new_val > 80) new_val = 81;        // Maximalwert begrenzen
  else if(new_val < 10) new_val = 9;     // Minimalwert begrenzen

  ir_left = (new_val + ir_left_prev) / 2;
}


void processStateMachine() {
  switch (state) {
    case STOPP:
      stop();
      break;
    case FORWARD:
      driveForward();
      break;
    case BACKWARD:
      driveBackward();
      break;
  }
}

void driveForward() {

  if (ir_front < FRONT_STOP) {
    state = BACKWARD;
  }
  if (ir_right > ir_left) {
    spd_left = 50;
    spd_right = BASE_SPEED;
  }

  if (ir_left > ir_right) {
    spd_left = BASE_SPEED;
    spd_right = 50;
  }

  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, HIGH);
  digitalWrite(MOTOR_R_BWD, LOW);
  analogWrite(MOTOR_L_SPEED, spd_left);
  analogWrite(MOTOR_R_SPEED, spd_right);
}

void driveBackward() {
  backward(BASE_SPEED);

  if (ir_front >= FRONT_CLEAR) {
    state = FORWARD;
  }
}


void checkButtons() {
  if (digitalRead(START_BUTTON) == LOW && state == STOPP)
    state = FORWARD;
  if (digitalRead(STOP_BUTTON) == LOW)
    state = STOPP;
}

/*
void forward(int speed) {

  digitalWrite(MOTOR_L_FWD, HIGH);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, HIGH);
  digitalWrite(MOTOR_R_BWD, LOW);
  analogWrite(MOTOR_L_SPEED, speed);
  analogWrite(MOTOR_R_SPEED, speed);
}
*/
void backward(int speed) {
  digitalWrite(MOTOR_L_FWD, LOW);
  digitalWrite(MOTOR_L_BWD, HIGH);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, HIGH);
  analogWrite(MOTOR_L_SPEED, speed);
  analogWrite(MOTOR_R_SPEED, speed);
}


void stop() {
  digitalWrite(MOTOR_L_FWD, LOW);
  digitalWrite(MOTOR_L_BWD, LOW);
  digitalWrite(MOTOR_R_FWD, LOW);
  digitalWrite(MOTOR_R_BWD, LOW);
  analogWrite(MOTOR_L_SPEED, 0);
  analogWrite(MOTOR_R_SPEED, 0);
}


// ===== HILFSFUNKTIONEN =====
void printStatus() {
  Serial.print("F: \t"); Serial.print(ir_front);
  Serial.print(" R: \t"); Serial.print(ir_right);
  Serial.print(" L: \t"); Serial.print(ir_left);
  Serial.print("  spd_L= \t"); Serial.print(spd_left);
  Serial.print(" spd_R= \t"); Serial.print(spd_right);
  Serial.print("  [");
  switch (state) {
    case STOPP:    Serial.print("STOPP");    break;
    case FORWARD:  Serial.print("FORWARD");  break;
    case BACKWARD: Serial.print("BACKWARD"); break;
  }
  Serial.println("]");
}