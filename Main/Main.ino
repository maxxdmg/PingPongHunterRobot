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
boolean leftVal = false;
boolean middle = false;
boolean rightVal = false;
boolean goalTrigger;
int goalTriggerPin = 11;
int rightLimitSwitchPin = 10; 
int leftLimitSwitchPin = 7;

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
  robo.motors = motors;
  robo.motors.motorShield.begin();
  robo.spd = 100;

  // set up environmental sensors
  goalTrigger = false;
  pinMode(goalTriggerPin, INPUT);
  pinMode(rightLimitSwitchPin, INPUT);
  pinMode(leftLimitSwitchPin, INPUT);
  
  Serial.begin(9600);  // start serial for output
  
}


void loop() {
  /*
  for (int i=0; i<16;i+=2) {
    Serial.print(data[i] + " ");
  }*/
  
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
  Serial.println(robo.state);
}


void handleFreeState(struct robot *robo) {  
  handleErrorLimitSwitches(robo); // handle error limit switch
  runLineSensor();
  if (!compareLineValues()) {
    changeState(robo, TRIGGERED);
  } else {
    moveForward(robo);
  }
  // verifySequence(robo);
  setNewLineValues(robo);
}


void handleTriggeredState(struct robot *robo) {
  if ((!middle && rightVal && leftVal) || (!middle && rightVal && !leftVal)) {
    changeState(robo, SHIFTRIGHT);
  } else if (!middle && !rightVal && leftVal) {
    changeState(robo, SHIFTLEFT);
  } else {
    changeState(robo, FREE);
  }
}


void handleShiftRightState(struct robot *robo) {
  zeroTurnRight(robo);
  int i = 0;
  while(true) {
    handleErrorLimitSwitches(robo);
    // timeout action
    if (i > 1200) {
      if (i % 100 < 5)
        turnRight(robo);
      else 
        moveForward(robo);
      Serial.println("LEFT TIME OUT EXCEEDED");
    }
    runLineSensor();
    if (middle) {
      changeState(robo, FREE);
      break;
    } /*else if (leftVal) {
      robo->state = SHIFTLEFT;
      break;
    }*/
    i++;
  }
  
  stopMotors(robo); // stop motors
  setNewLineValues(robo); // reset line values
}


void handleShiftLeftState(struct robot *robo) {
  zeroTurnLeft(robo);
  int i = 0;
  while(true) {
    handleErrorLimitSwitches(robo);
    // timeout action
    if (i > 1200) {
      if (i % 100 < 5)
        turnLeft(robo);
      else 
        moveForward(robo);
      Serial.println("LEFT TIME OUT EXCEEDED");
    }
    
    // normal action
    runLineSensor();
    if (middle) {
      changeState(robo, FREE);
      break;
    } /*else if (rightVal) {
      robo->state = SHIFTRIGHT;
      break;
    }*/

    i++;
  }
  
  stopMotors(robo); // stop motors
  setNewLineValues(robo); // reset line values
}
void runLineSensor() {
  /*
  for (int i=0; i<16; i+=2) {
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println();*/
  Wire.requestFrom(9, 16);
  while (Wire.available()) {
    data[t] = Wire.read();
    if (t < 15) t++;
    else t = 0;
  }
  if (data[0] <= linevalue || data[2] <= linevalue) leftVal = true;
  else leftVal = false;
  if (data[4] <= linevalue || data[6] <= linevalue || data[8] <= linevalue || data[12] <= linevalue) middle = true;
  else middle = false;
  if (data[10] <= linevalue || data[14] <= linevalue) rightVal = true;
  else rightVal = false;
  newLineValues[0] = leftVal;
  newLineValues[1] = middle;
  newLineValues[2] = rightVal;
  verifySequence();
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
boolean verifySequence() {
  if ((lineValues[0] && !lineValues[1] && !lineValues[2]) && (!newLineValues[0] && !newLineValues[1] && newLineValues[2])) {
    return 0;
  } else if ((!lineValues[0] && !lineValues[1] && !lineValues[2]) && (newLineValues[0] && !newLineValues[1] && !newLineValues[2])) {
    return 0;
  }
  return 1;
}

void handleErrorLimitSwitches(struct robot *robo) {
  if (digitalRead(rightLimitSwitchPin) || digitalRead(leftLimitSwitchPin)) {
    stopMotors(robo);
    moveReverse(robo);
    while(digitalRead(rightLimitSwitchPin) || digitalRead(leftLimitSwitchPin)) {
      Serial.println("HIT");  
    }
    zeroTurnRight(robo);
    for (int i=0; i<65; i++) {
      Serial.println("TURNING");
    }
  }
}

void changeState(struct robot *robo, states nextState) {
  robo->prevState = robo->state;
  robo->state = nextState;
}

void moveForward(struct robot *robo) {
  robo->motors.motorLeft->run(FORWARD);
  robo->motors.motorRight->run(FORWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void moveReverse(struct robot *robo) {
  robo->motors.motorLeft->run(BACKWARD);
  robo->motors.motorRight->run(BACKWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void turnLeft(struct robot *robo) {
  robo->motors.motorLeft->run(FORWARD);
  robo->motors.motorRight->run(RELEASE);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void turnRight(struct robot *robo) {
  robo->motors.motorLeft->run(RELEASE);
  robo->motors.motorRight->run(FORWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void zeroTurnLeft(struct robot *robo) {
  robo->motors.motorLeft->run(FORWARD);
  robo->motors.motorRight->run(BACKWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void zeroTurnRight(struct robot *robo) {
  robo->motors.motorLeft->run(BACKWARD);
  robo->motors.motorRight->run(FORWARD);
  
  robo->motors.motorLeft->setSpeed(robo->spd);
  robo->motors.motorRight->setSpeed(robo->spd);
}

void stopMotors(struct robot *robo) {
  robo->motors.motorLeft->run(RELEASE);
  robo->motors.motorRight->run(RELEASE);
  
  robo->motors.motorLeft->setSpeed(0);
  robo->motors.motorRight->setSpeed(0);
}
