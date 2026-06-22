// Encoder controls a 28BYJ-48 stepper via ULN2003.
// CW   -> speed up (eventually one direction at full pace)
// CCW  -> slow down, then reverse, then full pace the other way
// LED blinks faster as |speed| grows. Speed 0 = motor off, LED off.

// --- pins ---------------------------------------------------------------
const int PIN_CLK = 2;
const int PIN_DT  = 3;
const int PIN_LED = 6;
const int PIN_IN1 = 8;
const int PIN_IN2 = 9;
const int PIN_IN3 = 10;
const int PIN_IN4 = 11;

// --- speed control ------------------------------------------------------
int speed = 0;                 // steps/sec, signed; sign = direction
const int CLICK_STEP = 20;     // change per encoder click
const int MAX_SPEED  = 500;    // 28BYJ-48 stalls above ~600 sps

// --- half-step sequence for 28BYJ-48 -----------------------------------
const uint8_t SEQ[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1},
};
int seqIndex = 0;

// --- encoder state ------------------------------------------------------
int lastClkLevel;

// --- timing state -------------------------------------------------------
unsigned long lastStepMicros  = 0;
unsigned long lastBlinkMillis = 0;
bool          ledOn           = false;

// ------------------------------------------------------------------------
void applyStep(int idx) {
  digitalWrite(PIN_IN1, SEQ[idx][0]);
  digitalWrite(PIN_IN2, SEQ[idx][1]);
  digitalWrite(PIN_IN3, SEQ[idx][2]);
  digitalWrite(PIN_IN4, SEQ[idx][3]);
}

void releaseCoils() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void setup() {
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT,  INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  lastClkLevel = digitalRead(PIN_CLK);
  releaseCoils();

  Serial.begin(115200);
  Serial.println(F("ready, speed=0"));
}

void loop() {
  // 1. Read encoder ------------------------------------------------------
  int clk = digitalRead(PIN_CLK);
  if (clk != lastClkLevel && clk == LOW) {
    int dt = digitalRead(PIN_DT);
    if (dt != clk) {
      speed += CLICK_STEP;   // CW
    } else {
      speed -= CLICK_STEP;   // CCW
    }
    if (speed >  MAX_SPEED) speed =  MAX_SPEED;
    if (speed < -MAX_SPEED) speed = -MAX_SPEED;
    Serial.print(F("speed="));
    Serial.println(speed);
  }
  lastClkLevel = clk;

  // 2. Step the motor at the right rate ----------------------------------
  if (speed != 0) {
    unsigned long now = micros();
    unsigned long intervalUs = 1000000UL / (unsigned)abs(speed);
    if (now - lastStepMicros >= intervalUs) {
      lastStepMicros = now;
      // advance index forward for +speed, backward for -speed
      seqIndex = (seqIndex + (speed > 0 ? 1 : 7)) % 8;
      applyStep(seqIndex);
    }
  } else {
    releaseCoils();  // save power and let the rotor cool when idle
  }

  // 3. LED blink rate ----------------------------------------------------
  if (speed != 0) {
    // Larger |speed| -> shorter period. Clamp to 30..1000 ms.
    long period = 1000 - 2L * abs(speed);
    if (period < 30)   period = 30;
    if (period > 1000) period = 1000;

    unsigned long now = millis();
    if (now - lastBlinkMillis >= (unsigned long)period) {
      lastBlinkMillis = now;
      ledOn = !ledOn;
      digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
    }
  } else {
    digitalWrite(PIN_LED, LOW);
  }
}
