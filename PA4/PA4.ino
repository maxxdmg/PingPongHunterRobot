#define M_PI 3.14159265358979323846

// initialize first motor
int pinOut1 = 3;
int pinForward1 = 9;
int pinBackward1 = 8;

// initialize second motor
int pinOut2 = 6;
int pinForward2 = 4;
int pinBackward2 = 5;

int timeToSpin240 = 2000; 
int timeToSpin180 = 1300;
int Speed = 225;

void setup() {
  pinMode(pinOut1, OUTPUT);
  pinMode(pinForward1, OUTPUT);
  pinMode(pinBackward1, OUTPUT);

  pinMode(pinOut2, OUTPUT);
  pinMode(pinForward2, OUTPUT);
  pinMode(pinBackward2, OUTPUT);
}

void loop() { 
  // set motor speed
  analogWrite(pinOut1, Speed * 2);
  analogWrite(pinOut2, Speed);

  moveForward(); 

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
  digitalWrite(pinBackward2, LOW);
}
