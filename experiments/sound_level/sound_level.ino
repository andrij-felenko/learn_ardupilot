// Waveshare sound sensor (LM386). AOUT on A0.
// Reads peak-to-peak amplitude over a short window, prints number and
// a bar to Serial Monitor.

const int PIN_AOUT = A0;

const int  WINDOW_MS  = 50;   // sampling window length
const int  BAR_WIDTH  = 50;   // characters in the bar
const int  BAR_FULL_AT = 600; // amplitude value that fills the bar

void setup() {
  Serial.begin(115200);
  Serial.println(F("sound_level: ready"));
}

void loop() {
  unsigned long windowStart = millis();
  int sampleMin = 1023;
  int sampleMax = 0;

  // Sample as fast as possible for WINDOW_MS milliseconds.
  while (millis() - windowStart < WINDOW_MS) {
    int v = analogRead(PIN_AOUT);
    if (v < sampleMin) sampleMin = v;
    if (v > sampleMax) sampleMax = v;
  }

  int amplitude = sampleMax - sampleMin;

  // Print number, then a bar like: amp=123  ##########............
  Serial.print(F("amp="));
  if (amplitude < 100) Serial.print(' ');
  if (amplitude < 10)  Serial.print(' ');
  Serial.print(amplitude);
  Serial.print(F("  "));

  int barFill = (long)amplitude * BAR_WIDTH / BAR_FULL_AT;
  if (barFill > BAR_WIDTH) barFill = BAR_WIDTH;
  for (int i = 0; i < BAR_WIDTH; i++) {
    Serial.print(i < barFill ? '#' : '.');
  }
  Serial.println();
}
