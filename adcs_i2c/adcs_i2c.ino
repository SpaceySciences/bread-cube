#include <Wire.h>
 
//#define TRX_ADDR 0x04
//#define GPS_ADDR 0x06
//#define EPS_ADDR 0x08
//#define CAM_ADDR 0x0A
#define IMU_ADDR 0x0C

int number = 0;
int w = 0;
int x = 0;
int y = 0;
int z = 0;
 
void setup() {
 Serial.begin(9600);
 
 // initialize i2c as slave
 Wire.begin(IMU_ADDR);
 
 // define callbacks for i2c communication
 Wire.onReceive(handleReceive);
 Wire.onRequest(handleRequest);

 Serial.println("***Beginning of IMU Module.***");
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
    
    if (number == 0) { //prepare to return w
      w = -100 + (rand() % 200);
    } else if (number == 1) { //prepare to return x
      x = -100 + (rand() % 200);
    } else if (number == 2) { //prepare to return y
      y = -100 + (rand() % 200);
    } else if (number == 3) { //prepare to return z
      z = -100 + (rand() % 200);
    } //if-else
  } //while
} //handleReceive

// callback for sending data
void handleRequest() {
  Serial.print("Sending: ");
  if (number == 0) { //return w
    Serial.println(w);
    Wire.write(w);
  } else if (number == 1) { //return x
    Serial.println(x);
    Wire.write(x);
  } else if (number == 2) { //return y
    Serial.println(y);
    Wire.write(y);
  } else if (number == 3) { //return z
    Serial.println(z);
    Wire.write(z);
  }
} //handleRequest
