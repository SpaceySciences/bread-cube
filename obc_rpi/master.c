#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

// The I2C bus: This is for V2 Pi's. for V1 Model B, "dev/i2c-0"
static const char *DEV_NAME = "/dev/i2c-1";

int I2C_BUS;

void startI2C();
void connectHere(int addr);
void sendCommand(int addr, int val);

int main (int argc, char **argv) {
  startI2C();

  while (1) {
    int whichDevice = 0;
    printf("\nTo which device would you like to connect (integer)? ");
    scanf("%d", &whichDevice);
    if (whichDevice < 0) break;
    connectHere(whichDevice);

    int inputSignal = 0;
    printf("Please enter the signal to send to the device (integer): ");
    scanf("%d", &inputSignal);
    if (inputSignal < 0) break;
    sendCommand(whichDevice, inputSignal);
  } //while

  close(I2C_BUS);
  return 0;
} //main

/**
 *Initiates the I2C bus
 */
void startI2C() {
  //attempting to open the I2C bus as a R/W file
  printf("I2C: Connecting to bus... ");
  if ( (I2C_BUS = open(DEV_NAME, O_RDWR)) < 0 ) {
    printf("Failed to access device %d\n", DEV_NAME);
    exit(1);
  } else {
    printf("Success\n");
  } //if-else
} //startI2C

/**
 *Connects to the specified I2C address
 */
void connectHere(int addr) {
  //attempting to acquire bus access/talk to slave
  printf("I2C: Acquiring bus to 0x%x... ", addr);
  if ( ioctl(I2C_BUS, I2C_SLAVE, addr) < 0 ) {
    printf("Failed to acquire bus access/talk\n");
    exit(1);
  } else {
    printf("Success\n");
  } //if-else
} //connect

void sendCommand(int addr, int val) {
  unsigned char outBuf[16];
  outBuf[0] = val;

  //sending command to slave
  printf("I2C: Sending %d to 0x%x... ", val, addr);
  if ( write(I2C_BUS, outBuf, 1) == 1 ) {
    usleep(10000); //required with microcontroller communication
    val = -1;
    unsigned char inBuf[1];
    if ( read(I2C_BUS, inBuf, 1) == 1 ) {
      val = (int)inBuf[0];
      printf("Received %d\n", val);
    } else {
      printf("Failed to receieve.\n");
    } //if-else
  } else {
    printf("Failed to send command.\n");
  } //if-else
  usleep(10000);
} //sendCommand
