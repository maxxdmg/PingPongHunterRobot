#include <Wire.h>
#include "robot.h"
#define uchar unsigned char
#define COMP1 0
#define reversetime 240

// initialize line sensor
uchar t;
int linevalue = 244;
uchar data[16];
boolean lineValues[3] = {false, false, false};
boolean newLineValues[3] = {false, false, false};
boolean leftVal = false;
boolean middle = false;
boolean rightVal = false;

int goalTriggerPin = 9;
int rightLimitSwitchPin = 10; 
int leftLimitSwitchPin = 7;

int servoReversePin = 3;

// set up motors
robotMotors motors;
robot robo;

void setup() {
  // set up line sensor
  Wire.begin();
  t = 0;
  
  // set up line sensor data contaianers
  runLineSensor(); // run line sensor
  setNewLineValues(&robo);

  // init robo->state
  robo.motors = motors;
  robo.motors.motorShield.begin();
  robo.spd = 128;

  // set up environmental sensors
  
  pinMode(goalTriggerPin, INPUT);
  pinMode(rightLimitSwitchPin, INPUT);
  pinMode(leftLimitSwitchPin, INPUT);
  
  Serial.begin(9600);  // start serial for output
}


void loop() {
  switch(robo.state) {
    case CATCHLINE:
      runLineSensor();
      handleErrorLimitSwitches(&robo);
      // cheeck if a line has been seen
      if (leftVal || middle || rightVal) {
        if (leftVal || middle) {
          turnLeft(&robo);
        } else if (rightVal) {
          turnRight(&robo);
        }
        while (true) {
          handleErrorLimitSwitches(&robo);
          runLineSensor();
          if (!leftVal && middle && !rightVal) {
            changeState(&robo, FREE);
            break;
          }
        }
      } else {
        moveForward(&robo);  
      }
      break;
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
  //Serial.println(robo.state);
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
  if (leftVal && middle && rightVal) {
    zeroTurnLeft(robo);
    while (leftVal && rightVal) {
      runLineSensor();
    }
    setNewLineValues(robo);
    changeState(robo, FREE);
  } else if ((leftVal && !middle && !rightVal) || (leftVal && !middle && rightVal)) {
    changeState(robo, SHIFTLEFT);
  } else if ((!middle && rightVal && !leftVal)) {
    changeState(robo, SHIFTRIGHT);
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
    // normal action
    runLineSensor();
    // break condition
    if (middle) {                    //CHANGE THIS TOMORROW(CAUSES ROBOT TO GET STUCK)
      if (leftVal && rightVal) {
        Serial.println("HIT");
        continue;
      } else {
        Serial.println("OTHER");
        changeState(robo, FREE);
        break;
      }
    } else if(!leftVal && !middle && !rightVal) {
      changeState(robo, robo->prevState);
      break; 
    }
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
  //verifySequence();
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

void handleErrorLimitSwitches(struct robot *robo) {
  // handle goal limit switch
  if (digitalRead(goalTriggerPin)) {    
    runLineSensor();
    // check if robot is not on any line
    if (!leftVal && !middle && !rightVal) {
        moveReverse(robo);
        for (int i=0; i<10; i++) {
          Serial.println("Reversing from goal trigger at a wall");
        }
        // turn 135 degrees, head back to center
        zeroTurnRight(robo);
        for (int i=0; i<20; i++) {
          Serial.println("Turning right to head towards center from goal trigger at a wall");
        }
        changeState(robo, CATCHLINE);
    // otherwise arrived at goal
    } else {
      handleGoalLimitSwitch(robo);
      changeState(robo, FREE);
    }
  }

  // handle error limit switch
  while((digitalRead(rightLimitSwitchPin)) || (digitalRead(leftLimitSwitchPin))) {
    stopMotors(robo);
    for(int i = 0; i<75; i++){
      moveReverse(robo);
    }
    for (int i=0; i<40; i++) {
      zeroTurnRight(robo);
    }
  }

    /*while(digitalRead(rightLimitSwitchPin) && digitalRead(leftLimitSwitchPin))
    {
    zeroTurnRight(robo);
    for (int i=0; i<65; i++) {
      Serial.println("TURNING");
    }
  }
  */
}

void handleGoalLimitSwitch(struct robot *robo) {
  stopMotors(robo);
  for (int i=0; i<40; i++) {
    Serial.println("Stopping to shoot from goal trigger at a wall");
  }
  moveReverse(robo);
  for(int i=0; i<20; i++) {
    Serial.println("Reversing from goal trigger handler");
  }

  // turn right, 90ish degrees
  turnRight(robo);
  for (int i=0; i<20; i++) {
    Serial.println("Turning right from goal trigger handler");  
  }

  // move forward
  moveForward(robo);
  for (int i=0; i<20; i++) {
    Serial.println("Moving forward from goal trigger handler");
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
