#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

int MotorSpeed = 5;
Adafruit_INA219 ina219;

float lastCurrent;
float lastVoltage;
bool rightcheck;
bool leftcheck;
bool increase;


unsigned long time = millis();


void setup(void) 
{
  Serial.begin(9600);

  //pinMode(STEPPER_PIN_1, OUTPUT);
  //pinMode(STEPPER_PIN_2, OUTPUT);
  //pinMode(STEPPER_PIN_3, OUTPUT);
  //pinMode(STEPPER_PIN_4, OUTPUT);
  myStepper.setSpeed(1);

  while (!Serial) {
      // will pause Zero, Leonardo, etc until serial console opens
      delay(1);
  }
    
  Serial.println("Hello!");
  
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();
  

  Serial.println("Measuring voltage and current with INA219 ...");
}

void loop(void) 
{


  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  float avgLoadVoltage = 0;
  float avgCurrent = 0;
  float samplesC =0, samplesV =0;


  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);



  for (int x = 0; x < 150; x++){ //Get 150 samples
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
 
    samplesC = samplesC + current_mA;  //Add samples together
    samplesV = samplesV + loadvoltage;
  }
  avgLoadVoltage = samplesV/150.000;//Taking Average of Samples
  avgCurrent = samplesC/150.000;
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(avgLoadVoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(avgCurrent); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  move(avgCurrent, avgLoadVoltage);
  lastCurrent = avgCurrent;
  lastVoltage = avgLoadVoltage;
  delay(10000);
}
void move(float newCurrent, float newVoltage){
  int steps = 0;
  increase = false;
  
  while (increase == false && steps < 500){
    myStepper.step(-1);
    steps++;
    if(ina219.getCurrent_mA()< lastCurrent && ina219.getBusVoltage_V() < lastVoltage){
      increase = false;
    }
    if(ina219.getCurrent_mA() > lastCurrent && ina219.getBusVoltage_V() > lastVoltage){
      increase = true;
    }

    //Serial.print("New Current:       "); Serial.print(newCurrent); Serial.print(" Last Current:       "); Serial.println(lastCurrent);
    //Serial.print("New Voltage:       "); Serial.print(newVoltage); Serial.print(" Last Voltage:       "); Serial.println(lastVoltage);
    delay(10);
  }
  steps = 0;
  while (increase == false && steps < 500){
    myStepper.step(1);
    steps++;
    if(ina219.getCurrent_mA() < lastCurrent && ina219.getBusVoltage_V() < lastVoltage){
      increase = false;
    }
    if(ina219.getCurrent_mA() > lastCurrent && ina219.getBusVoltage_V() > lastVoltage){
      increase = true;
    }

    //Serial.print("New Current:       "); Serial.print(newCurrent); Serial.print(" Last Current:       "); Serial.println(lastCurrent);
    //Serial.print("New Voltage:       "); Serial.print(newVoltage); Serial.print(" Last Voltage:       "); Serial.println(lastVoltage);
    delay(10);
  }
  
  
}

void clockwise(){
 //1
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
  //2
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
  //3
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //4
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //5
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //6
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //7
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //8
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
}
void counterclockwise(){
  //1
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //2
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //3
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //4
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //5
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, LOW);
  delay(MotorSpeed);
  //6
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
  //7
  digitalWrite(STEPPER_PIN_4, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
  //8
  digitalWrite(STEPPER_PIN_4, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_1, HIGH);
  delay(MotorSpeed);
}
