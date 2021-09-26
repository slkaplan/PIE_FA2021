


char arr [2];
void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  arr[0] = "0";


}

void loop() {
    
    Serial.write(arr);
    delay(10);
    
  
}
