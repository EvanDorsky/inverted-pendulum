#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <TimerOne.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motor = AFMS.getMotor(1);

int pot = A1;
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
int potpos = 0;
static int setpotpos = 323; // with default 5V ADC reference
static int maxpos = 675;

// loop variables
static unsigned long lspeedtime = 0;
static unsigned long curspeedtime = 0;

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
  // potpos = analogRead(pot) - setpotpos;

  merr = setpos - mpos;
  mdir = merr < 0? BACKWARD : FORWARD;
  mspeed = abs(merr) > 255? 255 : abs(merr);
}

void setup() {
  AFMS.begin();

  attachInterrupt(0, Aevent, RISING);
  attachInterrupt(1, Bevent, CHANGE);

  Timer1.initialize(1e3);
  Timer1.attachInterrupt(control);
}

void loop() {
  motor->setSpeed(mspeed);
  motor->run(mdir);
}