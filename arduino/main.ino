#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <TimerOne.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A0;
int A = 2;
int B = 3;

// #define DEBUG

volatile unsigned long lasttime = 0;
volatile unsigned long curtime = 0;
volatile unsigned long pulsetime = 0;

// motor variables
volatile int mdir = 1;
volatile int mpos = 0;
int setpos = 100;
int mspeed = 0; // set speed
float mvel = 0; // measured velocity
int merr = 0; // motor position error

// pendulum variables
// #define deg90 = 688;
// #define deg0 439 // with 5V ADC reference and supply
// #define degmin90 = 198;
// #define kP M_PI/4.0*(1.0/(deg90 - deg0) + 1.0/(deg0 - degmin90))
#define kP .00641 // ADC to rad
#define kPosF 0.00013 // .00641*.2
// #define kPosF 0 // .00641*.2

// loop variables
static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

// control variables
int drivedir = 0;
#define T .001 // sec
volatile float thetak = 0;
volatile float thetak1 = 0;
volatile float Voutk = 0;
volatile float Voutk1 = 0;
volatile int mposk1 = 0;

float posFb = 0.0;

#define kMotDAC 21.25

// K (series compensator)
// float kZero = exp(-15.0*T);
// float kPole = exp(-1.0*T);
// float kD = 15.0/1.0*((1.0 - kZero)/(1.0 - kPole)); // Kc = 1

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

float theta0 = 0;
volatile int mposk = 0;

void control()
{
  mposk = mpos;
  posFb = mposk*kPosF; // kPosF determined by matlab
  if (pulsetime)
    mvel = mdir*1.0/((float)(pulsetime*32e-6))*0.016;
  thetak = analogRead(pot)*kP - theta0;

  Voutk = Voutk1*.99970004 + 7857.1779*(thetak - .99203191*thetak1) + posFb - .5*mvel;

  drivedir = Voutk < 0? BACKWARD : FORWARD;
  // Voutk = Voutk > 12? 12 : Voutk;

  thetak1 = thetak;
  Voutk1 = Voutk;
  mposk1 = mposk;
}

void setup() {
  // reset things
  mpos = 0;
  // theta0 = 3.26908;
  theta0 = analogRead(pot)*kP;
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  AFMS.begin();

  attachInterrupt(0, Aevent, RISING);
  attachInterrupt(1, Bevent, CHANGE);

  Timer1.initialize((int)(T*1e6));
  Timer1.attachInterrupt(control);
}

#ifdef DEBUG
  static unsigned long lastprint = 0;
  static unsigned long curprint = 0;
#endif

float driveV = 0;
unsigned int STOP = 0;

void loop() {
  #ifdef DEBUG
    curprint = millis();
    if (curprint - lastprint > 100) {
      Serial.print("Angle: ");
      Serial.print(thetak*10000);
      Serial.print(" Pos: ");
      Serial.print(mpos);
      Serial.print(" Vout: ");
      Serial.println(Voutk);
      lastprint = curprint;
    }
  #endif
  if ((thetak > 1.4 || thetak < -1.4) && (thetak1 > 1.4 || thetak1 < -1.4)) { // about 90deg (current and last)
    STOP = 1;
    motor->setSpeed(0);
    motor->run(RELEASE);
  }
  if (STOP == 0) {
    driveV = Voutk > 12? 12 : Voutk;
    driveV = Voutk < -12? -12 : Voutk;
    motor->setSpeed((int)(fabsf(driveV)*kMotDAC));
    motor->run(drivedir);
  }
}