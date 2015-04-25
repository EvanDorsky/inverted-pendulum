#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <TimerOne.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A0;
int A = 2;
int B = 3;

volatile unsigned long lasttime = 0;
volatile unsigned long curtime = 0;
volatile unsigned long pulsetime = 0;

// motor variables
volatile int mdir = 0;
volatile int mpos = 0;
int setpos = 100;
int mspeed = 0; // set speed
float mvel = 0; // measured velocity
int merr = 0; // motor position error

// pendulum variables
float potpos = 0;
float deg90 = 688;
float deg0 = 439; // with 5V ADC reference and supply
float degmin90 = 198;
float Kp = M_PI/4.0*(1.0/(deg90 - deg0) + 1.0/(deg0 - degmin90)); // ADC to rad

// loop variables
static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

// control variables
static float T = 1e-3; // sec

// K (series compensator)
float Kzero = exp(-15.0*T);
float Kpole = exp(-1.0*T);
float Kd = 15.0/1.0*((1.0 - Kzero)/(1.0 - Kpole)); // Kc = 1

void Aevent() {
  if (digitalRead(B) == HIGH) {
    mdir = -1;
    mpos--;
  } else {
    mdir = 1;
    mpos++;
  }
}

void Bevent() {
  curtime = micros();
  if (curtime > lasttime)
    pulsetime = curtime - lasttime;

  lasttime = curtime;
}

void control()
{
  // mvel = mdir*1.0/((float)(pulsetime*32e-6));
  potpos = (analogRead(pot) - deg0)*Kp;

  merr = setpos - mpos;
  mdir = merr < 0? BACKWARD : FORWARD;
  mspeed = abs(merr) > 255? 255 : abs(merr);
}

void setup() {
  AFMS.begin();
  Serial.begin(115200);

  attachInterrupt(0, Aevent, RISING);
  attachInterrupt(1, Bevent, CHANGE);

  Timer1.initialize((int)(T*1e6));
  Timer1.attachInterrupt(control);
}

static unsigned long lastprint = 0;
static unsigned long curprint = 0;

void loop() {
  curprint = millis();
  if (curprint - lastprint > 50) {
    Serial.println(potpos);
    lastprint = curprint;
  }

  motor->setSpeed(mspeed);
  motor->run(mdir);
}