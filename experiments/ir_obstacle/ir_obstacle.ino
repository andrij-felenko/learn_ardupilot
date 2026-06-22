// FC-51 / KY-032 IR obstacle sensor.
// OUT on pin 7. Built-in LED echoes the state.
// Prints to Serial only on transitions, so the monitor stays readable.

const int PIN_OUT = 7;
const int PIN_LED = LED_BUILTIN;

int lastState = HIGH;

void setup() {
  pinMode(PIN_OUT, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);
  Serial.println(F("ir_obstacle: ready"));
}

void loop() {
  int state = digitalRead(PIN_OUT);

  // Mirror to LED. Most modules pull OUT LOW when an object is detected.
  digitalWrite(PIN_LED, state == LOW ? HIGH : LOW);

  if (state != lastState) {
    Serial.println(state == LOW ? F("object detected") : F("clear"));
    lastState = state;
  }
}
