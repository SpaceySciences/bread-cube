#include <Wire.h>
 
#define TRX_ADDR 0x04
#define LED1 30
#define LED2 33

int number = 0;
bool isRecv = false; 
short recvSize = 0;
double temp;
 
void setup() {
 Serial.begin(9600);
 pinMode(LED1, OUTPUT);
 pinMode(LED2, OUTPUT);
 
 // initialize i2c as slave
 Wire.begin(TRX_ADDR);
 
 // define callbacks for i2c communication
 Wire.onReceive(handleReceive);
 Wire.onRequest(handleRequest);

 Serial.println("***Beginning of TRX Module.***");
}
 
void loop() {
 delay(100);
 temp = GetTemp();
}

// callback for received data
void handleReceive(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("Received: ");
    Serial.println(number);
    
    if (number == 0) { //prepare to return diagnostic data
      temp = GetTemp();
    } else if (number == 1) { //are you receiving data?
      isRecv = rand() % 2;
      Serial.println("is receiving");
    } else if (number == 2) { //how much data are you receiving?
      recvSize = rand() % 255;
    } else if (number == 3) { //send data from Ard to RPi over SPI
      //move data over SPI
    } else if (number == 4) { //transmit setup and prepare for SPI data from RPi to Ard
      //
    } else { //this represents the transmission size in bytes that RPi will tell via I2C
      
    } //if-else
  } //while
} //handleReceive

// callback for sending data
void handleRequest() {
  Serial.print("Sending: ");
  if (number == 0) { //return diagnostic data
    Serial.println((int)temp);
    Wire.write((int)temp);
  } else if (number == 1) { //return 1 if receiving data, 0 otherwise
    Serial.println((int)isRecv);
    Wire.write(isRecv);
  } else if (number == 2) { //return size of data being received
    Serial.println((int)recvSize);
    Wire.write(recvSize);
  } else {
    Serial.println(number);
    Wire.write(number);
  }
} //handleRequest
 
// Get the internal temperature of the arduino
double GetTemp(void) {
  return 20 + (rand() % 40);
// unsigned int wADC;
// double t;
// ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
// ADCSRA |= _BV(ADEN); // enable the ADC
// delay(20); // wait for voltages to become stable.
// ADCSRA |= _BV(ADSC); // Start the ADC
// while (bit_is_set(ADCSRA,ADSC));
// wADC = ADCW;
// t = (wADC - 324.31 ) / 1.22;
// return (t);
}