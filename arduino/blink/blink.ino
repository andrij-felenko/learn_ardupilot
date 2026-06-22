void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println(F("blink: boot"));
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(F("on"));
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("off"));
  delay(500);
}