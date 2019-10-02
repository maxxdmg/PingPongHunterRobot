#define M_PI 3.14159265358979323846

int Speed = 100; // ADJUST this to increase/decrease power
int Distance = 0;
int Angle = 0;

// initialize first motor
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;

// initialize second motor
int pinOut2 = 5;
int pinForward2 = 13;
int pinBackward2 = 12;

int timeToSpin240 = 800; // ADJUST how long to turn the robot counter clockwise 240, spins too short -> increase, spins too long -> decrease
int timeToSpin180 = 600; // ADJUST how long to turn the robot counter clockwise 240, spins too short -> increase, spins too long -> decrease

void setup() {
  
}

void loop() {
  moveForward(1000, Speed);

 /*
  rotateCounterClockwise(timeToSpin240, Speed);  
  delay(5);

  moveForward(1000, Speed);
  delay(5);

  rotateCounterClockwise(timeToSpin240, Speed);
  delay(5);

  moveForward(1000, Speed);
  delay(5);

  rotateCounterClockwise(timeToSpin180, Speed);
  delay(5);

  moveForward(1000, Speed);
  delay(5);

  rotateCounterClockwise(timeToSpin240, Speed);
  delay(5);

  moveForward(1000, Speed);
  delay(5);

  rotateCounterClockwise(timeToSpin240, Speed);
  delay(5);

  moveForward(1000, Speed);
  delay(5);  */
    
  delay(10000);
}


// set speed output to the set speed var
void setOutput(int spd) {
  // write speed output 
  analogWrite(pinOut2, spd);  
  analogWrite(pinOut1, spd);
}


// move robot forwards
void moveForward(int t, int spd) { 
  int s = 0;
  
  setOutput(0);
  
  // move first motor 
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  // acceleration stuff here
  while(s <= spd ) {
        s = s + 5; // ADJUST acceleration amount
        setOutput(s);
        delay(50); 
  }

  delay(t - 500); // ADJUST when to begin deceleration

  // deceleration stuff here
  while(s >= 0 ) {
        s = s - 5; // ADJUST deceleration amount
        setOutput(s);
        delay(50); 
  }

  setOutput(0);
}


// rotate robot counter clockwise 240 degrees
void rotateCounterClockwise(int t, int spd) {
  int s = 0;
  int currTime = 0;
  int prevTime = 0;

  setOutput(0);
  
  // move first motor
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);

  // move second motor move opposite
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  // acceleration stuff here
  while(s <= spd ) {
        s = s + 5; // ADJUST acceleration amount
        setOutput(s);
        delay(50); 
  }

  delay(t);

  // deceleration stuff here
  while(s >= 0 ) {
        s = s - 5; // ADJUST deceleration amount
        setOutput(s);
        delay(50); 
  }
  setOutput(0);
}
