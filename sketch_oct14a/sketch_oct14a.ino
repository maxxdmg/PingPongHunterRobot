#include <Wire.h>
#define uchar unsigned char
#define COMP1 0
#define M_PI 3.14159265358979323846

// initialize line sensor
uchar t;
int linevalue = 244;
uchar data[16];
boolean left = false;
boolean middle = false;
boolean right = false;
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;
int pinOut2 = 3;
int pinForward2 = 4;
int pinBackward2 = 5;
int Speed = 70;
enum states {
  FREE,
  TRIGGERED,
  SHIFTLEFT,
  SHIFTRIGHT,
  CENTERED
} state;

void setup() {
  // set up motors
  pinMode(pinOut1, OUTPUT);
  pinMode(pinForward1, OUTPUT);
  pinMode(pinBackward1, OUTPUT);
  pinMode(pinOut2, OUTPUT);
  pinMode(pinForward2, OUTPUT);
  pinMode(pinBackward2, OUTPUT);

  pinMode(13, OUTPUT);
  // set up line sensor
  Wire.begin();
  t = 0;
  state = FREE;
  analogWrite(pinOut1, Speed - COMP1);
  analogWrite(pinOut2, Speed);
  Serial.begin(9600);  // start serial for output
  runLineSensor();
}


void loop() { 
  switch(state) {
    case FREE:
      handleFreeState();
      break;
    case TRIGGERED:
      handleTriggeredState();
      break;
    case SHIFTRIGHT:
      handleShiftRightState();
      break;
    case SHIFTLEFT:
      handleShiftLeftState();
      break;
    default:
      break;
  } 
  
  //Serial.println(state);
}


void handleFreeState() {
  runLineSensor();
  if (left && middle && right) {
    moveForward();
  } else if ((left || right) && !middle) {
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
  if (left) {
    state = SHIFTLEFT;
  } else if (right) {
    state = SHIFTRIGHT;
  } else {
    state = FREE;
  }
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
  
  for (int i=0; i<16; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
}


void moveForward() {
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
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
