// HC-SR04 distance shown on a 5641AS 4-digit 7-segment display.
// Distance in cm. Range "blank" if no echo.

const int PIN_TRIG = 2;
const int PIN_ECHO = 3;

// segment pins, in order: a, b, c, d, e, f, g, dp
const int PIN_SEG[8] = {4, 5, 6, 7, 8, 9, 10, 11};
// digit-select pins, left to right: D1, D2, D3, D4
const int PIN_DIG[4] = {12, 13, A0, A1};

// Set to false if your display is common-anode (segments invert).
const bool COMMON_CATHODE = true;

// 7-segment patterns. Bit 0=a, 1=b, 2=c, 3=d, 4=e, 5=f, 6=g
const byte DIGITS_0_9[10] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111, // 9
};
const byte BLANK = 0;
const byte DASH  = 0b1000000;  // only segment g

int distanceCm = -1;
unsigned long lastMeasureMs = 0;
unsigned long lastDigitMs   = 0;
int activeDigit = 0;

void allDigitsOff() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(PIN_DIG[i], COMMON_CATHODE ? HIGH : LOW);
  }
}

void writeSegments(byte pattern, bool dp) {
  byte on  = COMMON_CATHODE ? HIGH : LOW;
  byte off = COMMON_CATHODE ? LOW  : HIGH;
  for (int i = 0; i < 7; i++) {
    digitalWrite(PIN_SEG[i], (pattern & (1 << i)) ? on : off);
  }
  digitalWrite(PIN_SEG[7], dp ? on : off);
}

void enableDigit(int idx) {
  digitalWrite(PIN_DIG[idx], COMMON_CATHODE ? LOW : HIGH);
}

byte patternForDigit(int idx) {
  if (distanceCm < 0) return DASH;

  int d3 = (distanceCm / 1000) % 10;
  int d2 = (distanceCm / 100)  % 10;
  int d1 = (distanceCm / 10)   % 10;
  int d0 =  distanceCm         % 10;

  // suppress leading zeros so "42" shows as "  42", not "0042"
  switch (idx) {
    case 0: return d3 ? DIGITS_0_9[d3] : BLANK;
    case 1: return (d3 || d2) ? DIGITS_0_9[d2] : BLANK;
    case 2: return (d3 || d2 || d1) ? DIGITS_0_9[d1] : BLANK;
    case 3: return DIGITS_0_9[d0];
  }
  return BLANK;
}

void measure() {
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  unsigned long us = pulseIn(PIN_ECHO, HIGH, 25000UL);
  distanceCm = (us == 0) ? -1 : (int)(us / 58);

  if (distanceCm < 0) {
    Serial.println(F("no echo"));
  } else {
    Serial.print(F("distance: "));
    Serial.print(distanceCm);
    Serial.println(F(" cm"));
  }
}

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  for (int i = 0; i < 8; i++) pinMode(PIN_SEG[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(PIN_DIG[i], OUTPUT);
  allDigitsOff();

  Serial.begin(115200);
  Serial.println(F("distance_display: ready"));
}

void loop() {
  // measure every 200 ms
  if (millis() - lastMeasureMs >= 200) {
    lastMeasureMs = millis();
    measure();
  }

  // refresh next digit every 3 ms => ~83 Hz total refresh, no flicker
  if (millis() - lastDigitMs >= 3) {
    lastDigitMs = millis();
    allDigitsOff();
    writeSegments(patternForDigit(activeDigit), false);
    enableDigit(activeDigit);
    activeDigit = (activeDigit + 1) % 4;
  }
}
