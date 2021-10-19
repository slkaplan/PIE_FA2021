#include <SoftwareSerial.h>
String Data = "";
SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
bool rok = false;
void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

}

void loop() {
  
}
  while (HC12.available())
      {
          char character = HC12.read(); // Receive a single character from the software serial port
          Data.concat(character); // Add the received character to the receive buffer
          if (character == '\n')
          {
              
              Serial.print(Data);
  
              // Add your code to parse the received line here....
  
              // Clear receive buffer so we're ready to receive the next line
              Data = "";
          }
  
  
  for (int i = 0; i <= 255; i++) {
    String radio_msg = "b"; 
    radio_msg = radio_msg + "," + i + "\n";
    char copy[50];
    radio_msg.toCharArray(copy, 50);
    //Serial.println(copy);
    HC12.write(copy);
    
    delay(1000);
    Serial.println(i);
  }
  
  
  
}
