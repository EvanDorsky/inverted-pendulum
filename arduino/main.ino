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
float deg90 = 688;
float deg0 = 439; // with 5V ADC reference and supply
float degmin90 = 198;
float kP = M_PI/4.0*(1.0/(deg90 - deg0) + 1.0/(deg0 - degmin90)); // ADC to rad

// loop variables
static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

// control variables
static float T = .001; // sec
float thetak = 0;
float thetak1 = 0;
float Voutk = 0;
float Voutk1 = 0;

float kMotDAC = 1.0;

// K (series compensator)
float kZero = exp(-15.0*T);
float kPole = exp(-1.0*T);
float kD = 15.0/1.0*((1.0 - kZero)/(1.0 - kPole)); // Kc = 1

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
  thetak = (analogRead(pot) - deg0)*kP;

  Voutk = Voutk1*kPole + kD*(thetak - thetak1*kZero);

  mdir = Voutk < 0? BACKWARD : FORWARD;

  thetak1 = thetak;
  Voutk1 = Voutk;
}

void setup() {
  // AFMS.begin(); higher freq

  // attachInterrupt(0, Aevent, RISING);
  // attachInterrupt(1, Bevent, CHANGE);

  Timer1.initialize((int)(T*1e6));
  Timer1.attachInterrupt(control);
}

static unsigned long lastprint = 0;
static unsigned long curprint = 0;

void loop() {
  motor->setSpeed(abs(Voutk)*kMotDAC);
  motor->run(mdir);
}