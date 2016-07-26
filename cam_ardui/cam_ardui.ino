#include <Wire.h>

//#define TRX_ADDR 0x04
//#define GPS_ADDR 0x06
//#define EPS_ADDR 0x08
#define CAM_ADDR 0x0A


int number = 0;
bool hasCaptured = false; 
int picSize = 0;
bool sending = false;
 
void setup() {
 Serial.begin(9600);
 
 // initialize i2c as slave
 Wire.begin(CAM_ADDR);
 
 // define callbacks for i2c communication
 Wire.onReceive(handleReceive);
 Wire.onRequest(handleRequest);

 Serial.println("***Beginning of CAM Module.***");
}
 
void loop() {
 delay(100);
}

// callback for received data
void handleReceive(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("Received: ");
    Serial.println(number);
    
    if (number == 0) { //capture image
      hasCaptured = rand() % 2;
    } else if (number == 1) { //how big is the image?
      picSize = rand() % 255;
    } else if (number == 2) { //send the image over SPI
      sending = true;
      //do SPI stuff
    } //if-else
  } //while
} //handleReceive

// callback for sending data
void handleRequest() {
  Serial.print("Sending: ");
  if (number == 0) { //prepare to capture image
    Serial.println((int)hasCaptured);
    Wire.write((int)hasCaptured);
  } else if (number == 1) { //how big is the image?
    Serial.println(picSize);
    Wire.write(picSize);
  } else if (number == 2) { //send the image over SPI
    Serial.println((int)sending);
    Wire.write((int)sending);
  } //if-else
} //handleRequest

