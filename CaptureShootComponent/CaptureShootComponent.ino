#import <Servo.h>

int servoLeftPin = 9;
int servoRightPin = 10;
int trigPin = 5;
int echoPin = 6;
int duration = 0;
int distance = 0;
int ballInCacheDistance = 5;
Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  // set pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // set servos
  servoLeft.attach(servoLeftPin);
  servoRight.attach(servoRightPin);
}

void loop() {
  flushInput();
  
  duration = pulseIn(echoPin, HIGH); // read input from echo
  
  distance = getDistance(duration); // convert time -> distance in cm

  // handle distances
  if (distance <= ballInCacheDistance) {
    // there is a ball in cache
    Serial.println("Ball in cache");
    servoShoot();
  } else {
    Serial.println("Cache empty");
    servoCapture();
  }
}

void servoStop () {
  servoLeft.write(0);
  servoRight.write(0);  
}

void servoShoot () {
  servoLeft.write(180);
  servoRight.write(180);
}

void servoCapture() {
  servoLeft.write(-180);
  servoRight.write(-180);
}


void flushInput() {
  // flush analog pulse measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);  

  // output trigPin for 10 ms
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
 
}

long getDistance(int dur) {
  int d = (dur / 2) / 29.1;  
  return d;
}
