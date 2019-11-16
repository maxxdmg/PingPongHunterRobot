#include <Adafruit_MotorShield.h>

#define RIGHT90_TIME 750          // time to take a 90 degree right turn
#define STATE_TIMEOUT 10000       // time to change states if no exit criteria met (stuck on something)
#define WALL_SENSOR_TRIGGER 10    // how close to a wall to be to take action
#define NORMAL_SPEED 200          // normal PWM for motors for a normal speed

struct robotMotors                // All the information needed for motors.  Could also be direction and speed.
{
  Adafruit_MotorShield motorShield = Adafruit_MotorShield();
  Adafruit_DCMotor *motorLeft = motorShield.getMotor(1);
  Adafruit_DCMotor *motorRight = motorShield.getMotor(2);
};

enum states {
  FREE,
  TRIGGERED,
  SHIFTLEFT,
  SHIFTRIGHT
};

struct robot {
  struct robotMotors motors;      // a struct inside a struct.  Robot now has motors.
  enum states state = FREE;      // initialize state to this
  int spd;
};
