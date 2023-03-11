/***Added for Debug***/
#include <Arduino.h>
#include <avr8-stub.h>
// #include <app_api.h> // only needed with flash breakpoints
#include <SoftwareSerial.h>

// There is only one uart per arduino UNO.
// In order to debug the arduino UNO, while
// keeping the serial communications, we can connect
// an arduino to another and read from the other's serial port
SoftwareSerial SWSerial(2, 3); // rx = 2, tx = 3


// If the debugger is not launching, make sure libncurses5
// is installed. sudo apt-get install libncurses5
/*********************/


void setup()
{
  debug_init(); // initialize GDB stub
  pinMode(LED_BUILTIN, OUTPUT);
  SWSerial.begin(9600);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  char buffer[] = "Hello";
  SWSerial.println(buffer);
}
