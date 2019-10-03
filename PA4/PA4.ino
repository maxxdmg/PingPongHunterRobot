#define M_PI 3.14159265358979323846

float Speed = 127.0; // ADJUST this to increase/decrease power

// initialize first motor
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;

// initialize second motor
int pinOut2 = 5;
int pinForward2 = 12;
int pinBackward2 = 13;

int timeToSpin240 = 100; // ADJUST how long to turn the robot counter clockwise 240, spins too short -> increase, spins too long -> decrease
int timeToSpin180 = 30; // ADJUST how long to turn the robot counter clockwise 240, spins too short -> increase, spins too long -> decrease

void setup() {
  pinMode(pinOut1, OUTPUT);
  pinMode(pinForward1, OUTPUT);
  pinMode(pinBackward1, OUTPUT);

  pinMode(pinOut2, OUTPUT);
  pinMode(pinForward2, OUTPUT);
  pinMode(pinBackward2, OUTPUT);
}

void loop() { 
  
  moveForward(3000, Speed);

  rotateCounterClockwise(Speed);
  
  moveForward(3000, Speed);

  rotateCounterClockwise(Speed);

  moveForward(3000, Speed);

  rotate180(Speed);

  moveForward(3000, Speed);

  rotateClockwise(Speed);

  moveForward(3000, Speed);

  rotateClockwise(Speed);

  analogWrite(pinOut1, 0);
  analogWrite(pinOut2, 0);
  delay(13000);
}


// move robot forwards
void moveForward(int t, float spd) { 
  float s = 0.0;
  
  analogWrite(pinOut2, 0);  
  analogWrite(pinOut1, 0);
  
  // move first motor 
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  // acceleration stuff here
  while(s <= spd ) {
        s = s + 1; // ADJUST acceleration amount
        analogWrite(pinOut2, s);
        analogWrite(pinOut1, s);  
        delayMicroseconds(100);      
  }

  // in b/w acceleration and deceleration
  analogWrite(pinOut2, spd);
  analogWrite(pinOut1, spd  + spd / 2);
  delay(t);

  // deceleration stuff here
  while(s >= 0 ) {
        s = s - 1; // ADJUST deceleration amount
        analogWrite(pinOut2, s);
        analogWrite(pinOut1, s);
        delayMicroseconds(100);    
  }

}


void rotateCounterClockwise(float spd) {  
  analogWrite(pinOut2, 0);
  analogWrite(pinOut1, 0);
  
  // move first motor
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);

  // move second motor move opposite
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  analogWrite(pinOut2, spd * 2);
  analogWrite(pinOut1, spd * 2);
  delay(1700);

}


void rotate180(float spd) { 
  analogWrite(pinOut2, 0);
  analogWrite(pinOut1, 0);
  
  // move first motor
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);

  // move second motor move opposite
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  analogWrite(pinOut2, spd * 2);
  analogWrite(pinOut1, spd * 2);
  delay(1300);
}


void rotateClockwise(float spd) {  
  analogWrite(pinOut2, 0);
  analogWrite(pinOut1, 0);
  
  // move first motor
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor move opposite
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);

  analogWrite(pinOut2, spd * 2);
  analogWrite(pinOut1, spd * 2);
  delay(1700);

}
