// KY-040 rotary encoder controls the blink frequency of the built-in LED.
// CW  -> faster blink, CCW -> slower blink.
// Open Serial Monitor at 115200 to see the current period.

// --- pin assignments ----------------------------------------------------
const int PIN_CLK = 2;          // encoder CLK
const int PIN_DT  = 3;          // encoder DT
const int PIN_LED = 6;//LED_BUILTIN; // built-in LED, wired internally to pin 13

// --- blink state --------------------------------------------------------
int blinkPeriod   = 500;   // ms; how long LED is on, and how long it is off
const int STEP    = 50;    // ms; one encoder click changes period by this
const int MIN_PER = 50;    // limits, so LED stays visible / not too slow
const int MAX_PER = 2000;

// --- encoder state ------------------------------------------------------
int lastClkLevel;

void setup() {
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT,  INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  lastClkLevel = digitalRead(PIN_CLK);

  Serial.begin(115200);
  Serial.print(F("ready, period="));
  Serial.println(blinkPeriod);
}

void loop() {
  // --- 1. read the encoder ---------------------------------------------
  int clk = digitalRead(PIN_CLK);

  // CLK changed compared to last loop iteration?
  // We only act on one of the two edges (the falling edge: HIGH -> LOW)
  // so we get exactly one count per detent.
  if (clk != lastClkLevel && clk == LOW) {
    int dt = digitalRead(PIN_DT);

    if (dt != clk) {
      // CW: speed up (smaller period)
      blinkPeriod -= STEP;
    } else {
      // CCW: slow down (larger period)
      blinkPeriod += STEP;
    }

    // clamp into [MIN_PER, MAX_PER]
    if (blinkPeriod < MIN_PER) blinkPeriod = MIN_PER;
    if (blinkPeriod > MAX_PER) blinkPeriod = MAX_PER;

    Serial.print(F("period="));
    Serial.println(blinkPeriod);
  }
  lastClkLevel = clk;

  // --- 2. blink the LED ------------------------------------------------
  // Trick: millis() / blinkPeriod gives an ever-growing integer.
  // Mod 2 makes it flip 0,1,0,1,... every blinkPeriod milliseconds.
  // No delay() — encoder keeps being read every loop iteration.
  digitalWrite(PIN_LED, (millis() / blinkPeriod) % 2);
}
