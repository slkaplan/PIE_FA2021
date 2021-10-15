int sensorLPin = A0;    // select the input pin for the potentiometer
int sensorRPin = A1;    // select the input pin for the potentiometer
int sensorLValue = 0;  // variable to store the value coming from the sensor
int sensorRValue = 0;
byte ledLPin = 7;
byte ledRPin = 8;
int loopTime = 20; // 50 milliseconds
int readings = 50;        // uses char 't' for total readings
int currReadings = 0;
unsigned long currTime = 0;
unsigned long lastTime = 0;
bool mosfetLOn = true;    // uses char 'l'
bool mosfetROn = true;    // uses char 'r'
bool pause = true;        // Uses char 'p'

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;


void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(ledLPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  digitalWrite(ledLPin, mosfetLOn);
  digitalWrite(ledRPin, mosfetROn);
}

void loop() {

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
        // this temporary copy is necessary to protect the original data
        //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
    char firstChar = messageFromPC[0];
    switch (firstChar) {
      case 'p':
        // pause
        pause = !pause;
        break;
      case 't':
        // statements
        readings = floatFromPC;
        break;
      case 'l':
        mosfetLOn = floatFromPC;
        break;
      case 'r':
        mosfetROn = floatFromPC;
        break;
      default:
        // statements
        Serial.print("Could not match '");
        Serial.print(messageFromPC);
        Serial.print("' to a variable");
        break;
    }

    // Now Apply the updates
    digitalWrite(ledLPin, mosfetLOn);
    digitalWrite(ledRPin, mosfetROn);
  }


  if(!pause){
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
    currReadings += 1;
  }
  delay(loopTime);
  
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
    bool gotData = false;
    while (Serial.available() > 0) {
      newData = true;
      rc = Serial.read();
      receivedChars[ndx] = rc;
      ndx++;
    }
    if(newData){
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
    }

}

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx);     // convert this part to a float

}


void showParsedData() {
  // UNcoment to show what is sent
//    Serial.print("Message ");
//    Serial.println(messageFromPC);
//    Serial.print("Float ");
//    Serial.println(floatFromPC);
}
