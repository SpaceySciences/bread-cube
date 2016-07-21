#define BATTERY_CAPACITY      2.4   // Battery capacity in Ah
#define BATTERY_VOLT_MAX      7.4
#define MAX_POWER_CONSUMPTION 20.0
#define POWER_GENERATION      7.0

long time = 0;
float delta = 0;
float batteryLevel = BATTERY_CAPACITY*BATTERY_VOLT_MAX;
boolean deployed = false;

void setup() {
  Serial.begin(9600);

  time = micros();

  pinMode(6, INPUT);
}

void loop() {


  deployed = !digitalRead(6);
  
  if(deployed){ 
  
    long newTime;
    // put your main code here, to run repeatedly:
    int pot = analogRead(A1);
    float consumption = pot/1023.0*MAX_POWER_CONSUMPTION;
    
    newTime = micros();
    delta = (newTime - time)/1000000.0/60/60;
    
    batteryLevel += delta*(POWER_GENERATION-consumption);
    time = newTime;
  
    if(batteryLevel < 0){
      batteryLevel = 0;
      Serial.println("Out of energy!");
    }
    Serial.println(batteryLevel);
  }
}
