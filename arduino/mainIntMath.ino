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
unsigned int deg90 = 688;
unsigned int deg0 = 439; // with 5V ADC reference and supply
unsigned int degmin90 = 198;
unsigned int kP = (unsigned int) ((M_PI/4.0*(1.0/(deg90 - deg0) + 1.0/(deg0 - degmin90)))*10000.0); // ADC to rad

// loop variables
static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

// control variables
static float T = .001; // sec
unsigned int thetak = 0;
unsigned int thetak1 = 0;
unsigned int Voutk = 0;
unsigned int Voutk1 = 0;

unsigned int kMotDAC = 1;

// K (series compensator)
unsigned int kZero = (unsigned int) (exp(-15.0*T)*1000.0);
unsigned int kPole = (unsigned int) (exp(-1.0*T)*1000.0);
unsigned int kD = (unsigned int) (15000.0/1000.0*((1000.0 - kZero)/(1000.0 - kPole))); // Kc = 1

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
  thetak = (analogRead(pot) - deg0)*kP/10; //makes thetak multiplied by 1000

  Voutk = Voutk1*kPole + kD*(thetak - thetak1*kZero); //all numbers are multipled by 1000

  mdir = Voutk < 0 ? BACKWARD : FORWARD;

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