#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#define STEPPER_PIN_1 8
#define STEPPER_PIN_2 9
#define STEPPER_PIN_3 10
#define STEPPER_PIN_4 11

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

Adafruit_INA219 ina219;

bool SkipLeft = false;
bool SkipRight = false;
int totalSteps = 0;


unsigned long time = millis();



struct averagePower{
 float current;
 float voltage;
 float power;
 float oldCurrent;
 float oldVoltage;
 float oldPower;
};

averagePower avgPower;

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
      delay(16);
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

void ReadCurrentSensor(){
  bool printSensor = true;
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


  //Filter data
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

  //Update avgPower
  avgPower.current = abs(avgCurrent);
  avgPower.voltage = avgLoadVoltage;
  avgPower.power = ina219.getPower_mW();

  if(printSensor){
    Serial.print("BusVoltage:"); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("ShuntVoltage:"); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("LoadVoltage:"); Serial.print(avgLoadVoltage); Serial.println(" V");
    Serial.print("Current:"); Serial.print(avgCurrent); Serial.println(" mA");
    Serial.print("Power:"); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");
  }
}

void loop(void) 
{

  updateSolarPanel();
  
  //Check every 10 secs
  delay(10000);
}
void updateSolarPanel(){
  //Keep track of steps for while loop
  int steps = 0;
  //Interval that determines a sensor check
  int stepInterval = 50;
  //The counter that, when equal to stepinterval allows a sensor check condition (resets to zero after condition is met)
  int stepIntervalCount = 0;
  //Allows an initial loop to pass to get old and new readings
  bool init = false;
  //(new sum of readings - old sum of readings) divided by stepInterval
  float rateOfIncrease = 0;
  //rateOfIncrease, stored after each loop
  float oldRateOfIncrease =0;

  //update readings, initialize old readings
  ReadCurrentSensor();
  avgPower.oldCurrent = avgPower.current;
  avgPower.oldVoltage = avgPower.voltage;
  avgPower.oldPower = avgPower.power;

//Clockwise loop (1024 steps = 180 degrees)

  while (stepInterval > 2 && steps < 1024){
    //Clockwise movement (-1) Counterclockwise movement (1)
    myStepper.step(-1);
    //Keep track of local steps
    steps++;
    //Keep track of global steps (for untangle cord function)
    totalSteps--;
    //Increment step interval count, for sensor check condition
    stepIntervalCount++;

    //When stepIntervalCount matches the stepInterval, run the sensor check
    if(stepIntervalCount == stepInterval){
      //Reset stepIntervalCount
      stepIntervalCount = 0;
      //Get new readings
      ReadCurrentSensor();
      //Update the rateOfIncrease
      rateOfIncrease = ((avgPower.current+avgPower.voltage+avgPower.power) - (avgPower.oldCurrent + avgPower.oldVoltage+avgPower.oldPower))/stepInterval;

      //check if the loop has initialized
      if(init){
        //The panel is moving and the readings are increasing at a faster rate than before, gradually increase the stepInterval to move the motor a bit more. The next check will be later.
        if(rateOfIncrease > oldRateOfIncrease){
          stepInterval = stepInterval+2;
        }
        //readings are not increasing at a faster rate
        else{
          //rateOfIncrease is not greater, but not zero. Keep the interval the same
          if(rateOfIncrease > 0){
            stepInterval = stepInterval;
          }
          //rateOfIncrease is decreasing, aggressively reduce the stepinterval, divide by 4. The next check will be sooner
          else{
            stepInterval = stepInterval/4;
          }
          
        }
      }
      //Print log debug
      Serial.print("rate of increase "); Serial.println(rateOfIncrease);
      Serial.print("avgPower.current "); Serial.print(avgPower.current); Serial.print(" avgPower.oldcurrent ");Serial.println(avgPower.oldCurrent);
      Serial.print("avgPower.voltage "); Serial.print(avgPower.voltage); Serial.print(" avgPower.oldvoltage ");Serial.println(avgPower.oldVoltage);

      //Set the old readings to the new ones for this check
      oldRateOfIncrease = rateOfIncrease;
      avgPower.oldCurrent = avgPower.current;
      avgPower.oldVoltage = avgPower.voltage;
      avgPower.oldPower = avgPower.power;
      init = true;

    }
    //Serial.print("New Current:       "); Serial.print(newCurrent); Serial.print(" Last Current:       "); Serial.println(lastCurrent);
    //Serial.print("New Voltage:       "); Serial.print(newVoltage); Serial.print(" Last Voltage:       "); Serial.println(lastVoltage);

  }



//CounterClockwise (Same as above opposite direction)

  steps = 0;
  stepInterval = 50;
  stepIntervalCount = 0;
  init = false;
  rateOfIncrease = 0;
  oldRateOfIncrease =0;
  ReadCurrentSensor();
  avgPower.oldCurrent = avgPower.current;
  avgPower.oldVoltage = avgPower.voltage;
  avgPower.oldPower = avgPower.power;

  while (stepInterval > 2 && steps < 1024){
    myStepper.step(1);
    steps++;
    totalSteps++;
    stepIntervalCount++;

    if(stepIntervalCount == stepInterval){
      stepIntervalCount = 0;
      ReadCurrentSensor();
      rateOfIncrease = ((avgPower.current+avgPower.voltage+avgPower.power) - (avgPower.oldCurrent + avgPower.oldVoltage+avgPower.oldPower))/stepInterval;

      if(init){
        if(rateOfIncrease > oldRateOfIncrease){
          stepInterval = stepInterval+2;
        }
        else{
          if(rateOfIncrease > 0){
            stepInterval = stepInterval;
          }
          else{
            stepInterval = stepInterval/4;
          }
          
        }
        
      }
      Serial.print("rate of increase "); Serial.println(rateOfIncrease);
      Serial.print("avgPower.current "); Serial.print(avgPower.current); Serial.print(" avgPower.oldcurrent ");Serial.println(avgPower.oldCurrent);
      Serial.print("avgPower.voltage "); Serial.print(avgPower.voltage); Serial.print(" avgPower.oldvoltage ");Serial.println(avgPower.oldVoltage);
      oldRateOfIncrease = rateOfIncrease;
      avgPower.oldCurrent = avgPower.current;
      avgPower.oldVoltage = avgPower.voltage;
      avgPower.oldPower = avgPower.power;
      init = true;
    }
  }
 



//Check for almost full rotation and untangle cord
  if(abs(totalSteps)>2048 ){ //(2048 = 360 degrees)
    while(totalSteps!=0){ //0 = starting position
      if(totalSteps<0){ // Rotated more than -360 step counterclockwise
        totalSteps++;
        myStepper.step(1); 
      }
      else{// Rotated more than 360 step clockwise
        totalSteps--;
        myStepper.step(-1);
      }
    }
    
  }
  
}





