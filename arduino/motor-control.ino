#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A1;
int A = 2;
int B = 3;
int encoderPos = 0;
int encoderLast = LOW;
int n = LOW;

int dir = FORWARD;
int error = 0;
static int set = 323; // with default 5V ADC reference
static int maxpos = 675;
int mspeed = 0;

static unsigned long switchtime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("DC Motor test");

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
  n = digitalRead(encoder0PinA);
   if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
       encoder0Pos--;
     } else {
       encoder0Pos++;
     }
     Serial.print (encoder0Pos);
     Serial.print ("/");
   } 
   encoder0PinALast = n;

  // Serial.println(millis()); // time when measurement started
  // Serial.println(pulseIn(A, HIGH)); // high pulse length

  // Serial.print("Pot: ");
  // Serial.print(analogRead(pot));
  // Serial.print(" ");
  // Serial.println(millis());
}