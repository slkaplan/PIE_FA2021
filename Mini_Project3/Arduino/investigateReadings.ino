int sensorLPin = A0;    // select the input pin for the potentiometer
int sensorRPin = A1;    // select the input pin for the potentiometer
int sensorLValue = 0;  // variable to store the value coming from the sensor
int sensorRValue = 0;
byte ledLPin = 7;
byte ledRPin = 8;
int loopTime = 20; // 50 milliseconds
int readings = 50;
int currReadings = 0;
unsigned long currTime = 0;
unsigned long lastTime = 0;
const bool mosfetLOn = true;
const bool mosfetROn = true;


void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(ledLPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  if(mosfetLOn){
    digitalWrite(ledLPin, HIGH);
  }
  if(mosfetROn){
    digitalWrite(ledRPin, HIGH);
  }
}

void loop() {

  currTime = millis();
  // read the value from the sensor:
  sensorLValue = analogRead(sensorLPin);
  sensorRValue = analogRead(sensorRPin);
  Serial.print(sensorLValue);
  Serial.print(",");
  Serial.print(sensorRValue);
  Serial.print(";");
  if(currReadings >= readings){
    Serial.println("");
    currReadings = 0;
  }
  delay(loopTime);
  currReadings += 1;
}
