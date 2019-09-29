#include <Servo.h>

int Speed = 128;

// initialize first motor
int pinOut1 = 6;
int pinForward1 = 7;
int pinBackward1 = 8;

// initialize second motor
int pinOut2 = 5;
int pinForward2 = 12;
int pinBackward2 = 13;



void setup() {
  
}

void loop() {
  beginOutput();
  
  moveForward();
}

// set speed output to the set speed var
void beginOutput() {
  Serial.println("speed set");
  
  // write speed output 
  analogWrite(pinOut1, Speed);
  analogWrite(pinOut2, Speed); 
}

// move robot forwards
void moveForward() {
  Serial.println("moving forward");

  // move first motor 
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
}

// move robot backwards
void moveBackward() {
  Serial.println("moving backward");

  // move first motor
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);

  // move second motor
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
}

// set speed output to null, not necessary but used to make robot pause
void endOutput() {
  Serial.println("speed off");

  // write zero output
  analogWrite(pinOut1, 0);
  analogWrite(pinOut2, 0);
}
