#include <SoftwareSerial.h>

#define p0 A0
#define p1 A1
#define p2 A2
#define p3 A3


String Data = "";
SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin
bool rok = false;
void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

}

void loop() {
  

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
      }

          
  int p0_val = analogRead(p0);
  delay(10);
  int p1_val = analogRead(p1);
  delay(10);
  int p2_val = analogRead(p2);
  delay(10);
  int p3_val = analogRead(p3);
  delay(10);

//  Serial.println(p0_val);
//  Serial.println(p1_val);
//  Serial.println(p2_val);
//  Serial.println(p3_val);

    

  String radio_msg = ""; 
  radio_msg = radio_msg + p0_val + "," + p1_val + "," + p2_val + "," + p3_val + "," + "\n";
  char copy[50];
  radio_msg.toCharArray(copy, 50);
  HC12.write(copy);
  
  
//  for (int i = 0; i <= 255; i++) {
//    String radio_msg = "b"; 
//    radio_msg = radio_msg + "," + i + "\n";
//    char copy[50];
//    radio_msg.toCharArray(copy, 50);
//    //Serial.println(copy);
//    HC12.write(copy);
//    
//    delay(1000);
//    Serial.println(i);
//  }
  
  
  

}
