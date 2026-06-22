// HC-SR04 ultrasonic distance sensor.
// Measures every 100 ms, prints distance in cm to Serial Monitor.

const int PIN_TRIG = 2;
const int PIN_ECHO = 3;

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  Serial.begin(115200);
  Serial.println(F("distance_serial: ready"));
}

void loop() {
  // 1. Send a 10 us HIGH pulse on TRIG -> module fires the chirp.
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // 2. Measure how long ECHO stays HIGH. Timeout 30 ms (~5 m).
  unsigned long durationUs = pulseIn(PIN_ECHO, HIGH, 30000UL);

  if (durationUs == 0) {
    // No echo received in time — nothing in range or sensor disconnected.
    Serial.println(F("no echo"));
  } else {
    long distanceCm = durationUs / 58;
    Serial.print(F("distance: "));
    Serial.print(distanceCm);
    Serial.println(F(" cm"));
  }

  delay(100);
}
