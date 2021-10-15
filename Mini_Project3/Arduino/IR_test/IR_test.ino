#define right A0
#define left A3

void setup() 
{
Serial.begin(9600); 
//pinMode(2,OUTPUT); 
//digitalWrite(2,HIGH);
}
void loop(){   
  
delay(250); 
int right_read = analogRead(right);
delay(250);
int left_read = analogRead(left);
Serial.print("right: ");
Serial.print(right_read);

Serial.print(" ");
 
Serial.print("left: ");
Serial.print(left_read);

Serial.println("");
}
