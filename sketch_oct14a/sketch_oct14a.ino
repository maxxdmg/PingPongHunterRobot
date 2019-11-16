#include <Wire.h>
#include "robot.h"
#define uchar unsigned char
#define COMP1 0

// initialize line sensor
uchar t;
int linevalue = 244;
uchar data[16];
boolean lineValues[3] = {false, false, false};
boolean newLineValues[3] = {false, false, false};
boolean left = false;
boolean middle = false;
boolean right = false;

// set up motors
robotMotors motors;
robot robo;

void setup() {
  // set up line sensor
  Wire.begin();
  t = 0;
  
  // set up line sensor data containers
  runLineSensor(); // run line sensor
  setNewLineValues(&robo);

  // init robo->state
  robo.state = FREE;
  robo.motors = motors;
  
  Serial.begin(9600);  // start serial for output
  
}


void loop() { 
  switch(robo.state) {
    case FREE:
      handleFreeState(&robo);
      break;
    case TRIGGERED:
      handleTriggeredState(&robo);
      break;
    case SHIFTRIGHT:
      handleShiftRightState(&robo);
      break;
    case SHIFTLEFT:
      handleShiftLeftState(&robo);
      break;
    default:
      break;
  } 
  
  //Serial.println(robo->state);
}


void handleFreeState(struct robot *robo) {
  runLineSensor();
  if (!compareLineValues()) {
    robo->state = TRIGGERED;
  } else {
    moveForward(robo);
  }
  verifySequence(robo);
  setNewLineValues(robo);
}


void handleTriggeredState(struct robot *robo) {
  if ((middle && right && left) || (!middle && right && left) || (!middle && right && !left)) {
    robo->state = SHIFTRIGHT;
  } else if (!middle && !right && left) {
    robo->state = SHIFTLEFT;
  } else {
    robo->state = FREE;
  }
}


void handleShiftRightState(struct robot *robo) {
  turnRight(robo);
  
  for (int i=0; i < 400; i++) {
    runLineSensor();
    if (middle) break;
  }
  
  stopMotors(robo); // stop motors
  setNewLineValues(robo); // reset line values
  robo->state = FREE;
}


void handleShiftLeftState(struct robot *robo) {
  turnLeft(robo);
  
  for (int i=0; i < 400; i++) {
    runLineSensor();
    if (middle) break;
  }
  
  stopMotors(robo); // stop motors
  setNewLineValues(robo); // reset line values
  robo->state = FREE;
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
  newLineValues[0] = left;
  newLineValues[1] = middle;
  newLineValues[2] = right;
}
boolean compareLineValues() {
  for (int i=0; i<3; i++) {
    if (newLineValues[i] != lineValues[i])
      return false;
  }
  return true;
}
void setNewLineValues(struct robot *robo) {
  for (int i=0; i<3; i++)
    lineValues[i] = newLineValues[i];
}
boolean verifySequence(struct robot *robo) {
  if ((lineValues[0] && !lineValues[1] && !lineValues[2]) && (!newLineValues[0] && !newLineValues[1] && newLineValues[2])) {
    setNewLineValues(robo);
    robo->state = FREE;
    return 0;
  }
  return 1;
}


void moveForward(struct robot *robo) {
  robo->motors.motorLeft->run(FORWARD);
  robo->motors.motorRight->run(FORWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void turnLeft(struct robot *robo) {
  robo->motors.motorLeft->run(FORWARD);
  robo->motors.motorRight->run(BACKWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void turnRight(struct robot *robo) {
  robo->motors.motorLeft->run(BACKWARD);
  robo->motors.motorRight->run(FORWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void stopMotors(struct robot *robo) {
  robo->motors.motorLeft->run(RELEASE);
  robo->motors.motorRight->run(RELEASE);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}
