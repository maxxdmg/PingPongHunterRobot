//-----------------------------------------------------------------------
// Scott Zuidema
// Original: 2/22/2018
// Updated: 2/25/2018 with motor control, fixes, and using DEFINEs instead
// of hard values so it is easier to update.
// EGR 107
// Structure and State Machine Example Code
// Sets up a framework for a robot to navigate an arena by turning right
// 90 degress at each wall.  Must incude robot.h by clicking the "New Tab"
// option in the Arduino Sketch IDE (below the Serial icon in Arduio Sketch).
//-----------------------------------------------------------------------
 
#define RIGHT90_TIME 750          // time to take a 90 degree right turn
#define STATE_TIMEOUT 10000       // time to change states if no exit criteria met (stuck on something)
#define WALL_SENSOR_TRIGGER 10    // how close to a wall to be to take action
#define NORMAL_SPEED 200          // normal PWM for motors for a normal speed

#define RIGHT_PWM_PIN 3           // motor pins for right motor
#define RIGHT_IN1_PIN 4
#define RIGHT_IN2_PIN 5
#define LEFT_PWM_PIN 11           // motor pins for left motor
#define LEFT_IN1_PIN 12
#define LEFT_IN2_PIN 13

struct robotMotors                // All the information needed for motors.  Could also be direction and speed.
{
  int leftPWM;
  int rightPWM;
};

struct robotSensors               // All the sensor values on the robot.
{
  int forward;
  int left[2];
  int right[2];
  int ir;
  int line;
};

enum states                       // All the needed states for the robot.
{
  START,
  FORWARD,
  RIGHT90,
  SCORE
};

struct Robot 
{
  struct robotMotors motors;      // a struct inside a struct.  Robot now has motors.
  struct robotSensors sensors;    // Robot now has sensors.
  enum states state = START;      // initialize state to this
};

//Prototype functions below
void updateSensors(struct Robot *robot);
void updateMotors(struct Robot *robot);


