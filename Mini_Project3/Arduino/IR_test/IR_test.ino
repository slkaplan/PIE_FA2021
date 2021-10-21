#define XR A0
#define R A1
#define L A2
#define XL A3

void setup() 
{
Serial.begin(9600); 
//pinMode(2,OUTPUT); 
//digitalWrite(2,HIGH);
}
void loop(){   
  
Serial.print(analogRead(XR));
Serial.print(" ");
Serial.print(analogRead(R));
Serial.print(" ");
Serial.print(analogRead(L));
Serial.print(" ");
Serial.print(analogRead(XL));
Serial.print(" ");
Serial.println("");
delay(10);
}
