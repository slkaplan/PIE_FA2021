#include <SoftwareSerial.h>

String Data = "";

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

float floatFromPC = 0.0;
int intFromPC = 1;

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
int int0 = 0;
int int1 = 0;
int int2 = 0;
int int3 = 0;
int int4 = 0;


char *strings[5]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;

int A = 0;
int B = 0;
int C = 0;
int D = 0;
int E = 0;

boolean newData = false;


int test = 0;
//============

void setup() {
    Serial.begin(9600);
    HC12.begin(9600);
//    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
//    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
//    Serial.println();
}

//============

void loop() {


  
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        useParsedData();
        newData = false;
    }


//      while (HC12.available())
//      {
//          char character = HC12.read(); // Receive a single character from the software serial port
//          Data.concat(character); // Add the received character to the receive buffer
//          if (character == '\n')
//          {
//              
//              //Serial.print(Data);
//
//              int str_len = Data.length() + 1; 
//
//              // Prepare the character array (the buffer) 
//              char char_array[str_len];
//              
//              // Copy it over 
//              Data.toCharArray(char_array, str_len);
//              
//              byte index = 0;
//              ptr = strtok(char_array, ",");  // delimiter
//              while (ptr != NULL)
//              {
//                strings[index] = ptr;
//                index++;
//                ptr = strtok(NULL, ",");
//              }
////              A = strings[0];
////              B = strings[1];
////              C = strings[2];
////              D = strings[3];
//              for (int n = 0; n < index; n++)
//               {  
//                  if(n==0){
//                    A=atoi(strings[n]);
//                      }
////                  if(n==1){B=strings[n]};
////                  if(n==2){C=strings[n]};
////                  if(n==3){D=strings[n]};
////                  Serial.print(n);
////                  Serial.print("  ");
////                  Serial.println(strings[n]);
//               }
//  
//              // Clear receive buffer so we're ready to receive the next line
//              Data = "";
//              //delay(500);
//          }
//      }
      
    
  }

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (HC12.available() > 0 && newData == false) {
        rc = HC12.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    A = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    B = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    C = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    D = atoi(strtokIndx);     // convert this part to an integer

    

//    strtokIndx = strtok(NULL, ",");
//    floatFromPC = atof(strtokIndx);     // convert this part to a float

}

//============

void useParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("A ");
    Serial.println(A);
    Serial.print("B ");
    Serial.println(B);

  
//  Serial.print("MESSAGE: ");
//  Serial.print(messageFromPC);
//  Serial.println("A: " + String(A));
//  Serial.println("B: " + String(B));
//  Serial.println("A: " + String(C));
//  Serial.println("B: " + String(D));
//  Serial.println();
//  delay(1000);
    
}
