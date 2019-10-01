#include <Servo.h>
#define M_PI 3.14159265358979323846

int Speed = 200;
int Distance = 0;
int Angle = 0;

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
  setOutput(Speed);
  rotateClockwise(120, Speed);
  setOutput(0);
  delay(10000);
}


// set speed output to the set speed var
void setOutput(int spd) {
  Serial.print("speed set to ");
  Serial.println(spd);
  
  // write speed output 
  analogWrite(pinOut1, spd);
  analogWrite(pinOut2, spd); 
}


// move robot forwards
void moveForward(int dist, int spd) {
  int t = spd / dist;
 
  Serial.println("moving forward");

  // move first motor 
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);

  delay(t);
}


// move robot backwards
void moveBackward(int dist, int spd) {
  int t = spd / dist;
  
  Serial.println("moving backward");

  // move first motor
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);

  // move second motor
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);

  delay(t);
}

// rotate robot clockwise
void rotateClockwise(int angle, int spd) {
  int t = ((angle * 60000) / (M_PI * 63.5)) / 200;
  
  Serial.print("rotating clockwise by: ");
  Serial.println(angle);

  Serial.println(t);

  // move first motor
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);

  // move second motor move opposite
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);

  delay(t);
}
