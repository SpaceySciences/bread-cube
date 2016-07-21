#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define EPSILON .5

Quaternion attitudeQuaternion;
MPU6050 imu;


bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

int bound(int x, int min, int max){
  if(x < min)
    return min;
  else if(x > max)
    return max;
  return x;
}

void setup() {
  Serial.begin(9600);
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  Serial.println("Initializing MPU6050...");
  imu.initialize();
  Serial.println("MPU6050 initialized.");
  // Attempt to connect to IMU
  while(!imu.testConnection()){
    Serial.println("Initializing MPU6050...");
    imu.initialize();
    Serial.println("MPU6050 initialized.");

    if(!imu.testConnection()){
      Serial.println("MPU6050 connection failed.");
    }
  }

  
  devStatus = imu.dmpInitialize();

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
      // turn on the DMP, now that it's ready
      Serial.println(F("Enabling DMP..."));
      imu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
      attachInterrupt(0, dmpDataReady, RISING);
      mpuIntStatus = imu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = imu.dmpGetFIFOPacketSize();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
  }
}


void loop() {

  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
      // other program behavior stuff here
      // .
      // .
      // .
      // if you are really paranoid you can frequently test in between other
      // stuff to see if mpuInterrupt is true, and if so, "break;" from the
      // while() loop to immediately process the MPU data
      // .
      // .
      // .
  }
  mpuInterrupt = false;
  mpuIntStatus = imu.getIntStatus();

  // get current FIFO count
  fifoCount = imu.getFIFOCount();

  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      // reset so we can continue cleanly
      imu.resetFIFO();
      Serial.println(F("FIFO overflow!"));

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // wait for correct available data length, should be a VERY short wait
      while (fifoCount < packetSize) fifoCount = imu.getFIFOCount();

      // read a packet from FIFO
      imu.getFIFOBytes(fifoBuffer, packetSize);
      
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;

      imu.dmpGetQuaternion(&attitudeQuaternion, fifoBuffer);

//      Serial.print("quat\t");
 //     Serial.print(attitudeQuaternion.w);
//      Serial.print("\t");
//      Serial.print(attitudeQuaternion.x);/
//      Serial.print("\t");
//      Serial.print(attitudeQuaternion.y);
//      Serial.print("\t");
//      Serial.println(attitudeQuaternion.z);
  }

  if(abs(attitudeQuaternion.x - .5) < EPSILON)
    analogWrite(5, 255-255.0/EPSILON*abs(attitudeQuaternion.x - .5));
  else
    analogWrite(5, 0);

  if(abs(attitudeQuaternion.y - .5) < EPSILON)
    analogWrite(6, 255-255.0/EPSILON*abs(attitudeQuaternion.y - .5));
  else
    analogWrite(6, 0);

  if(abs(attitudeQuaternion.z - .5) < EPSILON)
    analogWrite(7, 255-255.0/EPSILON*abs(attitudeQuaternion.z - .5));
  else
    analogWrite(7, 0);
}

