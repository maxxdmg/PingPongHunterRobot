#include <Wire.h>
#include <Servo.h>
#define uchar unsigned char
#define COMP1 -20
#define M_PI 3.14159265358979323846

// initialize line sensor
uchar t;
int linevalue = 243;
uchar data[16];
boolean cacheEmpty = true;
boolean left = false;
boolean middle = false;
boolean right = false;
boolean goal = false;
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;
int pinOut2 = 3;
int pinForward2 = 5;
int pinBackward2 = 4;
int Speed = 122;
int pinServo = 0;
int pinGoalSwitch = 10;

enum states {
  FREE,
  TRIGGERED,
  SHIFTLEFT,
  SHIFTRIGHT,
  CENTERED,
  GOAL,
  TURNQUARTER,
  TRIGGEREDBACK,
  SHOOT
} state;
Servo servo;


void setup() {
  // set up motors
  pinMode(pinOut1, OUTPUT);
  pinMode(pinForward1, OUTPUT);
  pinMode(pinBackward1, OUTPUT);
  pinMode(pinOut2, OUTPUT);
  pinMode(pinForward2, OUTPUT);
  pinMode(pinBackward2, OUTPUT);
  pinMode(pinGoalSwitch, INPUT);
  
  Wire.begin();
  t = 0;
  
  state = FREE;
  
  analogWrite(pinOut1, Speed - COMP1);
  analogWrite(pinOut2, Speed);
  
  servo.attach(pinServo);
  
  Serial.begin(9600);  // start serial for output
}


void loop() { 
  switch(state) {
    case FREE:
      handleFreeState();
      break;
    case TRIGGERED:
      handleTriggeredState();
      break;
    case TRIGGEREDBACK:
      handleTriggeredBackState();
      break;
    case SHIFTRIGHT:
      handleShiftRightState();
      break;
    case SHIFTLEFT:
      handleShiftLeftState();
      break;
    case SHOOT:
      handleShootState();
      break;
    case GOAL:
      handleGoalState();
      break;
    case TURNQUARTER:
      handleTurnState();
    default:
      state = FREE;
      break;
  } 
  Serial.println(state);  
}


void handleFreeState() {
  runLineSensor();
  if (digitalRead(pinGoalSwitch)) {
    state = GOAL;
    goal = true;
    return;
  } else if (right && middle && left) {
    analogWrite(pinOut1, Speed * 2 - COMP1);
    analogWrite(pinOut2, Speed * 2);
    while (right && middle && left) {
      runLineSensor();
      moveForward();
    }
    analogWrite(pinOut1, Speed - COMP1);
    analogWrite(pinOut2, Speed);
    return;
  } else if (left || right) {
    state = TRIGGERED;
    stopMotors();
    return;
  } else {
    moveForward();
  }
}


void handleTriggeredState() {
  runLineSensor();
  /*
  Serial.print(left);
  Serial.print(middle);
  Serial.println(right); 
  */
  if (right && left) {
    state = FREE;
  } else if (left) {
    state = SHIFTLEFT;
  } else if (right) {
    state = SHIFTRIGHT;
  } else {
    state = FREE;
  }
}


void handleTriggeredBackState() {
  runLineSensor();
  /*
  Serial.print(left);
  Serial.print(middle);
  Serial.println(right); 
  */
  if (right && left) {
    state = GOAL;
  } else if (left) {
    state = SHIFTRIGHT;
  } else if (right) {
    state = SHIFTLEFT;
  } else {
    state = GOAL;
  }
}


/*
void handleGoalState() {
  moveBackward();
  while (!(right && middle && left)) {
      runLineSensor();
      if (right && middle && left) break;
  }
  stopMotors(); 
  state = TURNQUARTER;
}*/

void handleGoalState() {
  runLineSensor();
  moveBackward();
  if (right && middle && left) {
    state = TURNQUARTER;
    goal = false;
    return;
  } else if (left || right) {
    state = TRIGGEREDBACK;
    stopMotors();
    return;
  } else {
    moveBackward();
  }
}


void handleTurnState() {
  turnRight();
  for (int i=0; i<500; i++) {
    Serial.println(i);
  }
  stopMotors();
  turnRight();
  while (!(middle && left && right)) {
    if (middle && (right || left)) break;
  }
  state = FREE;
}


void handleShiftRightState() {
  // while the leftmost sensors do not see a line, turn right
  analogWrite(pinOut1, Speed + 16 - COMP1);
  analogWrite(pinOut2, Speed + 16);
  turnRight();
  for (int i=0; i < 400; i++) {
    runLineSensor();
    if (data[6] <= linevalue || data[8] <= linevalue) break;
    //Serial.println(i);
  }
  analogWrite(pinOut1, Speed - COMP1);
  analogWrite(pinOut2, Speed);
  stopMotors();
  state = FREE;
  if (goal) state = GOAL;
}


void handleShiftLeftState() {
  // while the leftmost sensors do not see a line, turn right
  analogWrite(pinOut1, Speed + 16 - COMP1);
  analogWrite(pinOut2, Speed + 16);
  turnLeft();
  for (int i=0; i < 400; i++) {
    runLineSensor();
    if (data[6] <= linevalue || data[8] <= linevalue) break;
    //Serial.println(i);
  }
  analogWrite(pinOut1, Speed - COMP1);
  analogWrite(pinOut2, Speed);
  stopMotors();
  state = FREE;
  if (goal) state = GOAL;
}


void runLineSensor() {
  digitalWrite(13, LOW);
  Wire.requestFrom(9, 16);
  digitalWrite(13, HIGH);
  while (Wire.available()) {
    data[t] = Wire.read();
    if (t < 15) t++;
    else t = 0;
  }

  if (data[0] <= linevalue || data[2] <= linevalue) left = true;
  else left = false;

  if (data[4] <= linevalue || data[6] <= linevalue || data[8] <= linevalue || data[10] <= linevalue) middle = true;
  else middle = false;

  if (data[14] <= linevalue || data[12] <= linevalue) right = true;
  else right = false;
  /*
  for (int i=0; i<16; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println(""); */
}


void handleShootState() {
  stopMotors();
  servo.write(70);

  analogWrite(pinOut1, Speed - COMP1 - 16);
  analogWrite(pinOut2, Speed - 16);
  moveForward();
  for (int i=0; i<100; i++);

  moveBackward();
  for (int i=0; i<300; i++);
  stopMotors();
}

void moveForward() {
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
}

void moveBackward() {
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
}

void turnLeft() {
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
}

void turnRight() {
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
}

void stopMotors() {
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, LOW);
}
