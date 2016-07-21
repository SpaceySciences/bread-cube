# bread-cube
A breadboard mockup of I2C and SPI interfaces expected to be on
SPOC and MOCI. Currently, Arduinos are acting as each subsystem,
and a Raspberry Pi Model B+ V1.2.

# examples
Example code was obtained from the following:
<https://blog.retep.org/2014/02/15/connecting-an-arduino-to-a-raspberry-pi-using-i2c/>

# libraries
The Arduinos require the Wire library, which is included in the
main directory if it isn't already packaged with the Arduino
IDE installation.

# arduinos
Currently, all subsystems are being emulated or are over simplified.

TRX	   --> 		  Arduino Mega ATMEGA2560
ADCS	   --> 		  Arudino Pro Mini
GPS	   -->		  Arduino Uno
N1CU	   -->		  Arduino Uno
EPS	   -->		  Arduino Leo Pro Micro

# rasberry-pi
The Raspberry Pi is acting as the On-Board Computer for the
satellites. Though the processor isn't the same as what will be on
the satellites, it has roughly similar ARM architecture.