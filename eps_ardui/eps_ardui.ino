#include <Wire.h>

#define BATTERY_CAPACITY      2.4   // Battery capacity in Ah
#define BATTERY_VOLT_MAX      7.4
#define MAX_POWER_CONSUMPTION 2000.0 //20.0
#define POWER_GENERATION      700//7.0

#define EPS_ADDR 0x08

long time = 0;
float delta = 0;
float batteryLevel = BATTERY_CAPACITY*BATTERY_VOLT_MAX;
boolean deployed = false;
long newTime = 0;
int pot = 0;
float consumption = 0;

int number = 0;
bool powerOn = true;

void setup() {
  Serial.begin(9600);

  time = micros();

  pinMode(6, INPUT);

  // initialize i2c as slave
  Wire.begin(EPS_ADDR);

  // define callbacks for i2c communication
  Wire.onReceive(handleReceive);
  Wire.onRequest(handleRequest);

  Serial.println("*** Beginning of EPS Module.***");
}

void loop() {
  deployed = !digitalRead(6);
  
  if(deployed){ 
    calcBatteryLevel();
    
    if(batteryLevel < 0){
      batteryLevel = 0;
      Serial.println("Out of energy!");
    }
    Serial.println(batteryLevel);
  }

  delay(100);
}

void calcBatteryLevel() {
  if (powerOn) {
    pot = analogRead(A1);
    consumption = pot/1023.0*MAX_POWER_CONSUMPTION;
  } else {
    consumption = 0;
  } //if-else
  
  newTime = micros();
  delta = (newTime - time)/1000000.0/60/60;
  
  batteryLevel += delta*(POWER_GENERATION-consumption);
  time = newTime;
} //calcBatterLevel

// callback for received data
void handleReceive(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("Received: ");
    Serial.println(number);
    
    if (number == 0) { //prepare to return power level
      calcBatteryLevel();
    } else if (number == 1) { //turn on
      powerOn = true;
    } else if (number == 2) {
      powerOn = false;
    } //if-else
  } //while
} //handleReceive

// callback for sending data
void handleRequest() {
  Serial.print("Sending: ");
  if (number == 0) { //return power level
    Serial.println(batteryLevel);
    Wire.write((int)batteryLevel);
  } else if (number == 1) { //return 1 if success, 0 otherwise
    Serial.println((int)powerOn);
    Wire.write((int)powerOn);
  } else if (number == 2) { //return 1 if success, 0 otherwise
    Serial.println((int)powerOn);
    Wire.write((int)powerOn);
  }
} //handleRequest
