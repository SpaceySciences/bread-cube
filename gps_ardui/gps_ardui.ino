/****************************/
/** Locsys MC-1513 **/
/****************************/

//https://www.sparkfun.com/tutorials/176

#include <Wire.h>

#define GPS_ADDR 0x06

int number = 0;
int x;
int y;
int z;

void setup() {
  Serial.begin(57600); 

  // initialize i2c as slave
  Wire.begin(GPS_ADDR);

  // define callbacks for i2c communication
  Wire.onReceive(handleReceive);
  Wire.onRequest(handleRequest);

  Serial.println("***Beginning of GPS Module.***");
} //setup

void loop() {
  if (Serial.available()) {
    for (int i = 0; i < 37; i++) {
      #if ARDUINO >= 100 //For Arduino v1.0+
      Serial.write(Serial.read());
      #else //For Arduino v0023 or earlier
      Serial.print(Serial.read(), BYTE);
      #endif
    } //for
  } //if
  Serial.println();
  delay(1000);
} //loop

void handleReceive(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("Received: ");
    Serial.println(number);
    
    if (number == 0) { //prepare to return x
      x = rand() % 180;
    } else if (number == 1) { //prepare to return y
      y = rand() % 180;
    } else if (number == 2) { //prepare to return z
      z = rand() % 180;
    } //if-else
  } //while
} //handleReceive

void handleRequest() {
  Serial.print("Sending: ");
  if (number == 0) { //return x
    Serial.println(x);
    Wire.write(x);
  } else if (number == 1) { //return y
    Serial.println(y);
    Wire.write(y);
  } else if (number == 2) { //return z
    Serial.println(z);
    Wire.write(z);
  } //if-else
} //handleRequest

