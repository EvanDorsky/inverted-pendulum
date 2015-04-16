#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A1;
int A = 2;
int B = 3;

volatile unsigned long starttime = 0;
volatile unsigned long curtime = 0;
volatile unsigned long pulsetime = 0;
volatile int motordir = 0;
volatile int pos = 0;

void Arising() {
  curtime = micros();
  if (curtime > starttime)
    pulsetime = curtime - starttime;

  starttime = curtime;

  if (digitalRead(B) == LOW) {
    motordir = -1;
    pos--;
  } else {
    motordir = 1;
    pos++;
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test");

  AFMS.begin();

  attachInterrupt(A, Arising, RISING);
}

int dir = FORWARD;
int error = 0;
static int setpos = 323; // with default 5V ADC reference
static int maxpos = 675;
int mspeed = 0; // set speed
float mvel = 0;// measure velocity

void loop() {
  error = setpos - analogRead(pot);
  dir = (error > 0)? FORWARD : BACKWARD;
  // mspeed = abs((float)error/maxpos*255);

  // motor->setSpeed(255);
  // motor->run(dir);

  mvel = motordir*1.0/((float)(pulsetime*16e-6));
  Serial.print("pos: ");
  Serial.print(pos);
  Serial.print(" vel: ");
  Serial.println(mvel);
}