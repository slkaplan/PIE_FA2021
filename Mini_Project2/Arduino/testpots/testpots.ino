#include <Servo.h>

Servo pan_servo;
Servo tilt_servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pan_servo.attach(9);
  tilt_servo.attach(10);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    int pot1 = analogRead(A1);
    int pot2 = analogRead(A2);

    pot1 = map(pot1, 0, 1023, 0, 180);
    pot2 = map(pot2, 0, 1023, 0, 180);

    Serial.print("Pot 1:");
    Serial.print(pot1);
    Serial.print(",    Pot 2:");
    Serial.print(pot2);
    Serial.println("");

    pan_servo.write(pot1);
    tilt_servo.write(pot2);

    delay(200);
}