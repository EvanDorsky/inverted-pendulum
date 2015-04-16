#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

//in hertz
#define ENCODER_FREQ 10000
#define VELOCITY_FREQ 20

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A1;
int A = 2;
int B = 3;
unsigned long encoderPos = 0;
unsigned long encoderTimer = 0;
unsigned long velocityTimer = 0;
volatile int encoderLast = LOW;
volatile int n = LOW;
volatile int nLast = 0;
volatile int velocity = 0;


int dir = FORWARD;
int error = 0;
static int set = 323; // with default 5V ADC reference
static int maxpos = 675;
int mspeed = 0;

static unsigned long switchtime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test");
  encoderTimer = micros();
  velocityTimer = millis();

  AFMS.begin();
}

void loop() {
  error = set - analogRead(pot);
  dir = (error > 0)? FORWARD : BACKWARD;
  // mspeed = abs((float)error/maxpos*255);

  motor->setSpeed(255);
  motor->run(dir);

  Serial.print("Speed: ");
  Serial.println(mspeed);

  //===== READING THE ENCODER ======
  // from http://playground.arduino.cc/Main/RotaryEncoders#Example1
  // doesn't use interrupts
  if(micros() - encoderTimer > 1000000/ENCODER_FREQ)
  {
	  encoderTimer = micros();
	  n = digitalRead(A);
	   if ((encoderLast == LOW) && (n == HIGH)) {
	     if (digitalRead(B) == LOW) {
	       encoderPos--;
	     } else {
	       encoderPos++;
	     }
	   } 
	   encoderLast = n;
   }

	if(millis() - velocityTimer > 1000/VELOCITY_FREQ)
	{
	  velocityTimer = millis();
	  //in clicks per millisecond?
	  velocity = (n - nLast)*VELOCITY_FREQ;
	  nLast = n;
    }

  // Serial.println(millis()); // time when measurement started
  // Serial.println(pulseIn(A, HIGH)); // high pulse length

  // Serial.print("Pot: ");
  // Serial.print(analogRead(pot));
  // Serial.print(" ");
  // Serial.println(millis());
}