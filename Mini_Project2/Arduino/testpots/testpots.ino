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
    int pot_pan = analogRead(A1);
    int pot_tilt = analogRead(A2);

    pot_pan = map(pot_pan, 0, 1023, 0, 180);
    pot_tilt = map(pot_tilt, 0, 1023, 0, 180);

    Serial.print("Pan: ");
    Serial.print(pot_pan);
    Serial.print(",    Tilt: ");
    Serial.print(pot_tilt);
    Serial.println("");

    pan_servo.write(pot_pan);
    tilt_servo.write(pot_tilt);

    delay(200);
}
