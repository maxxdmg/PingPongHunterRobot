#include <Wire.h>
#define uchar unsigned char
#define M_PI 3.14159265358979323846

// initialize line sensor
uchar t;
int linevalue = 246;
uchar data[16];

// initialize first motor
int pinOut1 = 6;
int pinForward1 = 9;
int pinBackward1 = 8;

// initialize second motor
int pinOut2 = 3;
int pinForward2 = 4;
int pinBackward2 = 5;

int Speed = 120;

enum states {
  FREE,
  TRIGGERED,
  SHIFTLEFT,
  SHIFTRIGHT,
  STRAIGHTEN,
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

  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);

  // set up line sensor
  Wire.begin();
  t = 0;
  state = FREE;
  analogWrite(pinOut1, Speed * 1.5);
  analogWrite(pinOut2, Speed);
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
    case CENTERED:
      state = FREE;
      break;
    case SHIFTRIGHT:
      handleStateShiftRight();
      break;
    case STRAIGHTEN:
      handleStateStraighten();
      break;
    case SHIFTLEFT:
      handleStateShiftLeft();
      break;
    default:
      break;
  }
  //Serial.println(state);
}


void handleFreeState() {
  runLineSensor();
  int i = 0;
  for (i=0; i<16; i++){
    if (data[i] <= linevalue) {
      state = TRIGGERED;
      return;
    }
    i++;
  }
  
  moveForward();
}


void handleTriggeredState() {
  stopMotors();
  
  // check for QTR sensors being centered
  if (data[6] <= linevalue || data[8] <= linevalue)
    state = CENTERED;
  else {
    // check left QTR sensors
    int i = 0;
    while (i < 6) {
      if (data[i] <= linevalue) {
        state = SHIFTLEFT;
        break;
      }
      i+=2;
    }
  
    // check right QTR sensors
    i = 14;
    while (i > 8) {
      if (data[i] <= linevalue) {
        state = SHIFTRIGHT;
        break;
      }
      i-=2;
    }
  }
}


void handleStateShiftRight() {
  // while the leftmost sensors do not see a line, turn right
  while(data[0] > linevalue && data[2] > linevalue && data[4] > linevalue && data[6] > linevalue) {
    runLineSensor();
    turnRight();
  }
  stopMotors();
  state = STRAIGHTEN;
}


void handleStateShiftLeft() {
  // while the rightmost sensors do not see a line, turn left
  while(data[14] > linevalue && data[12] > linevalue && data[10] > linevalue && data[8] > linevalue) {
    runLineSensor();
    turnLeft();
  }
  stopMotors();
  state = STRAIGHTEN;
}


void handleStateStraighten() {
  // while the middle sensors do not see a line, straighten to the left
  while(data[6] > linevalue && data[8] > linevalue) {
    runLineSensor();
    turnLeft();
  }
  stopMotors();
  state = CENTERED;
}


void runLineSensor() {
  Wire.requestFrom(9, 16);
  while (Wire.available()) {
    data[t] = Wire.read();
    if (t < 15) t++;
    else t = 0;
  }

  for (int i=0; i<16; i++) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println("");
  delay(1000);
}


void moveForward() {
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
}

void turnRight() {
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
}

void turnLeft() {
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
