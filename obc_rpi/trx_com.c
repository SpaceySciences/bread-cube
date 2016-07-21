#include "trx_com.h"

char file;

int main(int argc, char** argv) {
  connect();

  switch (argv[1][0]) {
  case '0':
    getDiagnostics();
    break;
  case '1':
    if (trxIsReceiving()) {
      printf("Yes incoming transmission\n");
      sleep(1);
      printf("Incoming data size is: %d bytes\n", (int)getDataSize());
    } else {
      printf("No incoming transmission\n");
    } //if-else
    break;
  default:
    printf("Argument failure\n");
  } //switch
   
  close(file);
  return (EXIT_SUCCESS);
}

void connect() {
  printf("I2C: Connecting\n");
 
  if ((file = open(devName, O_RDWR)) < 0) {
    fprintf(stderr, "I2C: Failed to access %d\n", devName);
    exit(1);
  }
 
  printf("I2C: acquiring bus to 0x%x\n", TRX_ADDR);
 
  if (ioctl(file, I2C_SLAVE, TRX_ADDR) < 0) {
    fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", TRX_ADDR);
    exit(1);
  }
} //connect

void getDiagnostics() {
  printf("Temperature is: %d\n", (int)sendCommand(0));
} //getDiagnostics

int trxIsReceiving() {
 return sendCommand(1);
} //trxIsReceiving

int getDataSize() {
  return sendCommand(2);
} //getDataSize

int sendCommand(int val) {
  unsigned char cmd[16];
  cmd[0] = val;
  //int write(  int  handle,  void  *buffer,  int  nbyte );
  //returns # bytes read
  if ( write(file, cmd, 1) == 1 ) {
    usleep(10000); //needed when communicating with microcontroller
    val = 0;
    unsigned char buf[1];

    //int  read(  int  handle,  void  *buffer,  int  nbyte );
    //returns # bytes read
    if ( read(file, buf, 1) == 1 ) {
      val = buf[0];
    } //if
  } //if
  
  usleep(10000); //needed when communicating with microcontroller
  return val;
} //sendCommand
