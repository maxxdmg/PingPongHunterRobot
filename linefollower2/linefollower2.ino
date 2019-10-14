#include <Wire.h>
#include <Servo.h>
#define uchar unsigned char
#define COMP1 20
#define M_PI 3.14159265358979323846

// initialize line sensor
uchar t;
int linevalue = 245;
uchar data[16];
boolean cacheEmpty = true;
boolean left = false;
boolean middle = false;
boolean right = false;
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;
int pinOut2 = 3;
int pinForward2 = 4;
int pinBackward2 = 5;
int Speed = 75;
int pinServo = 9;
int pinTrig = 11;
int pinEcho = 12;
int distance = 0;
int duration = 0;
int cachedistance = 16;
enum states {
  FREE,
  TRIGGERED,
  SHIFTLEFT,
  SHIFTRIGHT,
  CENTERED,
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
  pinMode(pinServo, OUTPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
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
      servo.write(110);
      break;
    case TRIGGERED:
      handleTriggeredState();
      servo.write(110);
      break;
    case SHIFTRIGHT:
      handleShiftRightState();
      servo.write(110);
      break;
    case SHIFTLEFT:
      handleShiftLeftState();
      servo.write(110);
      break;
    case SHOOT:
      handleShootState();
      break;
    default:
      break;
  } 

  Serial.println(state);  
}


void handleFreeState() {
  if(!checkCache()) {
    Serial.println("HIT");
    state = SHOOT;
    return;
  }
  runLineSensor();
  if (left && middle && right) {
    analogWrite(pinOut1, Speed + 16 - COMP1);
    analogWrite(pinOut2, Speed + 16);
    while (left && middle && right) {
      moveForward();
    } 
    runLineSensor();
    analogWrite(pinOut1, Speed - COMP1);
    analogWrite(pinOut2, Speed);
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
    Serial.println(i);
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
  
  while (!checkCache()) {
    Serial.println("Cache not empty");
  }
  stopMotors();
}


int checkCache() {
    flushInput();
    duration = pulseIn(pinEcho, HIGH);
    distance = getDistance(duration);
  
    Serial.print("Distance: ");
    Serial.println(distance);

  if (distance < cachedistance) return 0;
  return 1;
}


void flushInput() {
  // flush analog pulse measurement
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);  

  // output trigPin for 10 ms
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinTrig, LOW);
}
 

long getDistance(int dur) {
  long d = (dur / 2) / 29.1;  
  return d;
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
