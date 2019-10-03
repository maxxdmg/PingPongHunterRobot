#define M_PI 3.14159265358979323846

// initialize first motor
int pinOut1 = 6;
int pinForward1 = 8;
int pinBackward1 = 7;

// initialize second motor
int pinOut2 = 5;
int pinForward2 = 12;
int pinBackward2 = 13;

int timeToSpin240 = 2000; 
int timeToSpin180 = 1300;
float Speed = 127.0; // ADJUST this to increase/decrease power

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
  analogWrite(pinOut1, Speed);
  analogWrite(pinOut2, Speed);
  
  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);

  // rotate counter clockwise 240
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
  delay(timeToSpin240);

  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);

  // rotate counter clockwise 240
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
  delay(timeToSpin240);

  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);

  // rotate counter clockwise 180
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, LOW);
  digitalWrite(pinBackward2, HIGH);
  delay(timeToSpin180);


  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);

  // rotate clockwise 240
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(timeToSpin240);

  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);

  // rotate clockwise 240
  digitalWrite(pinForward1, LOW);
  digitalWrite(pinBackward1, HIGH);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(timeToSpin240);


  // move forward
  digitalWrite(pinForward1, HIGH);
  digitalWrite(pinBackward1, LOW);
  digitalWrite(pinForward2, HIGH);
  digitalWrite(pinBackward2, LOW);
  delay(3000);
  
  delay(13000);
}
