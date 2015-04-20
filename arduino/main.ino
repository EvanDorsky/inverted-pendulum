#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A1;
int A = 2;
int B = 3;

volatile unsigned long lasttime = 0;
volatile unsigned long curtime = 0;
volatile unsigned long pulsetime = 0;

volatile int motordir = 0;
volatile int pos = 0;

void Aevent() {
  if (digitalRead(B)) {
    motordir = -1;
    pos--;
  } else {
    motordir = 1;
    pos++;
  }
}

void Bevent() {
  curtime = micros();
  if (curtime > lasttime)
    pulsetime = curtime - lasttime;

  lasttime = curtime;
}

int dir = BACKWARD;
int error = 0;
static int setpos = 323; // with default 5V ADC reference
static int maxpos = 675;
int mspeed = 0; // set speed
float mvel = 0;// measure velocity

static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test");

  AFMS.begin();

  attachInterrupt(0, Aevent, RISING);
  attachInterrupt(1, Bevent, CHANGE);
  motor->run(dir);
}

void loop() {
  curspeedtime = millis();
  if (curspeedtime - lspeedtime > 4e3) {
    mspeed += 42;

    lspeedtime = curspeedtime;
    motor->setSpeed(mspeed);

    dir = dir == FORWARD? BACKWARD : FORWARD;
    motor->run(dir);
    Serial.print("Speed: ");
    Serial.print(mspeed);
    Serial.println("======================");
  }

  mvel = motordir*1.0/((float)(pulsetime*32e-6));

  // Serial.print("pos: ");
  // Serial.print(pos);
  Serial.print(" vel: ");
  Serial.println(mvel);
}