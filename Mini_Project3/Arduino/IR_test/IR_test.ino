void setup() 
{
Serial.begin(9600); 
pinMode(2,OUTPUT); 
digitalWrite(2,HIGH);
}
void loop(){   
  
delay(500); 
int a=analogRead(A0);
Serial.println(a); 

}
