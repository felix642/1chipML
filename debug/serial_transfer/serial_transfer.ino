#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); // rx = 2, tx = 3

void setup() 
{
  Serial.begin(9600, SERIAL_8N1);
  mySerial.begin(9600);
}

void loop() 
{
  // Serial to mySerial
  if(Serial.available() > 0 && mySerial.availableForWrite() > 0) {
    char x = Serial.read();
    Serial.print(x);
    Serial.println();
    mySerial.print(x);
  }
  
  // mySerial to Serial
  if(mySerial.available() > 0 && Serial.availableForWrite() > 0) {
    char y = mySerial.read();
    Serial.print(y);
  }
}