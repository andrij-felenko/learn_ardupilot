// KY-008 laser module on pin 13. Blinks 1 s on / 1 s off.

const int laserPin = 13;

void setup() {
  pinMode(laserPin, OUTPUT);
}

void loop() {
  digitalWrite(laserPin, HIGH);
  delay(1000);
  digitalWrite(laserPin, LOW);
  delay(1000);
}
