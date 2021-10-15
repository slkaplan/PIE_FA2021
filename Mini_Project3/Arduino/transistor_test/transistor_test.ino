#define testPin 2

void setup() {
  pinMode(testPin, OUTPUT);
}

void loop() {
  digitalWrite(testPin, HIGH);
  delay(5000);
  digitalWrite(testPin, LOW);
  delay(5000);

}
