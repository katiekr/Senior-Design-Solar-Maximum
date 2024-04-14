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

unsigned long time = millis();

struct positionData{
  int totalSteps;
  int bestLocationStep;
  bool skipClockwise;
  bool skipCounterClockwise;
  float maxReading;
  float counterClockwiseMax;
  float clockwiseMax;
  bool init;

};


struct averagePower{
 float current;
 float shuntVoltage;
 float busVoltage;
 float loadVoltage;
 float power;

 float oldCurrent;
 float oldVoltage;
 float oldPower;
};

averagePower avgPower;
positionData posData;
bool globalInit;

int addSteps = 2;
int divideSteps = 4;
int startingSteps = 40;

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
  unsigned long startTime = millis(); // Store the start time
  unsigned long elapsedTime = 0; // Variable to keep track of elapsed time
  int samples = 0;



  bool printSensor = true;
  float shuntVoltage = 0;
  float busVoltage = 0;
  float current_mA = 0;
  float loadVoltage = 0;
  float power_mW = 0;

  float samplesSV =0, samplesBV =0, samplesC = 0, samplesLV = 0, samplesP = 0;

  // Enter the while loop that runs for 2 seconds and Filter data
  while (elapsedTime < 500) { // 500 milliseconds = 0.5 seconds
    shuntVoltage = ina219.getShuntVoltage_mV();
    busVoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadVoltage = busVoltage + (shuntVoltage / 1000);

    samplesSV = samplesSV + shuntVoltage;
    samplesBV = samplesBV + busVoltage;
    samplesC = samplesC + current_mA;
    samplesLV = samplesLV + loadVoltage;
    samplesP = samplesP + power_mW;


    // Update the elapsed time
    elapsedTime = millis() - startTime;
    samples++;
  }

  //Update avgPower
  avgPower.shuntVoltage = samplesSV/samples;
  avgPower.busVoltage = samplesBV/samples;
  avgPower.current = samplesC/samples;
  avgPower.loadVoltage = samplesLV/samples;
  avgPower.power = samplesP/samples;

  //Instant Readings
  shuntVoltage = ina219.getShuntVoltage_mV();
  busVoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadVoltage = busVoltage + (shuntVoltage / 1000);

/*
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
*/
  if(printSensor){
    Serial.print("BusVoltage:"); Serial.print(busVoltage,6); Serial.println(" V");
    Serial.print("ShuntVoltage:"); Serial.print(shuntVoltage,6); Serial.println(" mV");
    Serial.print("LoadVoltage:"); Serial.print(loadVoltage, 6); Serial.println(" V");
    Serial.print("Current:"); Serial.print(current_mA,6); Serial.println(" mA");
    Serial.print("Power:"); Serial.print(power_mW,6); Serial.println(" mW");
    Serial.println("");

    Serial.print("AvgBusVoltage:"); Serial.print(avgPower.busVoltage,6); Serial.println(" V");
    Serial.print("AvgShuntVoltage:"); Serial.print(avgPower.shuntVoltage,6); Serial.println(" mV");
    Serial.print("AvgLoadVoltage:"); Serial.print(avgPower.loadVoltage ,6); Serial.println(" V");
    Serial.print("AvgCurrent:"); Serial.print(avgPower.current,6); Serial.println(" mA");
    Serial.print("AvgPower:"); Serial.print(avgPower.power,6); Serial.println(" mW");
    Serial.println("");
  }
}

void loop(void) 
{
  /*
  float tempReading;
  float maxThreshold = 0.20;
  float maxReading = posData.maxReading - (posData.maxReading * maxThreshold);

  ReadCurrentSensor();
  tempReading= (avgPower.current*avgPower.loadVoltage+(avgPower.power*2));
  
  if(maxReading > tempReading && globalInit == false){
    Serial.println("movement" + String(maxReading) + " > Read Value: " + String(tempReading)  );
    updateSolarPanel();
    globalInit == true;
   
  }
  else{
    Serial.println("No movement" + String(maxReading) + " < Read Value: " + String(tempReading)  );
  }
 */ 
  //ReadCurrentSensor();
  //Check every 5 secs
  updateSolarPanel();
  delay(5000);
}
void updateSolarPanel(){
  int cSteps = 0;
  int cCSteps = 0;



  if(posData.init == true){
    if(!posData.skipClockwise){
      Serial.println("Skipping CounterClockwise");
      Clockwise();
      posData.init = false;
    }
    else{
      Serial.println("Skipping Clockwise");
      CounterClockwise();
      posData.init = false;
    }
  }
  else{
    cSteps = Clockwise();
    cCSteps = CounterClockwise();
    posData.init = true;
  }

  if((cSteps > cCSteps) && (posData.init)){
    posData.skipClockwise = false;
    posData.skipCounterClockwise = true;
    Serial.println("More steps clockwise cSteps: " + String(cSteps) + " > cCSteps: " +  String(cCSteps));

  }
  else if(posData.init){
    posData.skipClockwise = true;
    posData.skipCounterClockwise = false;
    Serial.println("More steps counterclockwise Steps: " + String(cSteps) + " < cCSteps: " +  String(cCSteps));
  }

//Check for almost full rotation and untangle cord
  if(abs(posData.totalSteps)>2048 ){ //(2048 = 360 degrees)
    while(posData.totalSteps!=0){ //0 = starting position
      if(posData.totalSteps<0){ // Rotated more than -360 step counterclockwise
        posData.totalSteps++;
        myStepper.step(1); 
      }
      else{// Rotated more than 360 step clockwise
        posData.totalSteps--;
        myStepper.step(-1);
      }
    }
    
  }
  
}

int Clockwise(){
    //Keep track of steps for while loop
  int ClockwiseSteps = 0;
  //Interval that determines a sensor check
  int stepInterval = startingSteps;
  //The counter that, when equal to stepinterval allows a sensor check condition (resets to zero after condition is met)
  int stepIntervalCount = 0;
  //Allows an initial loop to pass to get old and new readings
  bool init = false;
  //(new sum of readings - old sum of readings)/stepInterval
  float rateOfIncrease = 0;
  //rateOfIncrease, stored after each loop
  float oldRateOfIncrease =0;
  //new sum of readings
  float sumofData =0;
  float oldsumofData = 0;

  //update readings, initialize old readings
  ReadCurrentSensor();
  avgPower.oldCurrent = avgPower.current;
  avgPower.oldVoltage = avgPower.loadVoltage;
  avgPower.oldPower = avgPower.power;

  //Clockwise loop (1024 steps = 180 degrees)

  while (stepInterval > 2 && ClockwiseSteps < 1024){
    //Clockwise movement (-1) Counterclockwise movement (1)
    myStepper.step(-1);
    //Keep track of local steps
    ClockwiseSteps++;
    //Keep track of global steps (for untangle cord function)
    posData.totalSteps--;
    //Increment step interval count, for sensor check condition
    stepIntervalCount++;

    //When stepIntervalCount matches the stepInterval, run the sensor check
    if(stepIntervalCount == stepInterval){
      //Reset stepIntervalCount
      stepIntervalCount = 0;
      //Get new readings
      ReadCurrentSensor();
      //Update the sumofData
      sumofData = (avgPower.current+avgPower.loadVoltage+(avgPower.power*2));
      rateOfIncrease = (sumofData - oldsumofData)/stepInterval;

      //check if the loop has initialized
      if(init){
        //The panel is moving and the readings are increasing at a faster rate than before, gradually increase the stepInterval to move the motor a bit more. The next check will be later.
        if(rateOfIncrease >= oldRateOfIncrease){
          stepInterval = stepInterval+addSteps;
        }
        //readings are not increasing at a faster rate
        else{
          if(rateOfIncrease > 0){
            stepInterval = stepInterval;
          }
          else{
            stepInterval = stepInterval/divideSteps;
          }

        }
      }
      //Print log debug
      Serial.print("rate of increase "); Serial.println(rateOfIncrease,6);
      Serial.print("old rate of increase "); Serial.println(oldRateOfIncrease,6);
      Serial.print("sumOfData "); Serial.println(sumofData,6);
      Serial.print("sumOfData Equation "); Serial.println("power: "+ String(avgPower.power) +" + current:"+ String(avgPower.current) +" + Lvoltage"+ String(avgPower.loadVoltage) + "/" + String(stepInterval));
      Serial.print("oldsumofData "); Serial.println(oldsumofData,6);
      
      //Set the old readings to the new ones for this check
      oldRateOfIncrease = rateOfIncrease;
      oldsumofData = sumofData;
      avgPower.oldCurrent = avgPower.current;
      avgPower.oldVoltage = avgPower.loadVoltage;
      avgPower.oldPower = avgPower.power;
      init = true;
      if(posData.maxReading < sumofData){
        posData.maxReading = sumofData;
      }

    }
    
  }
  return ClockwiseSteps;
}


int CounterClockwise(){
  //CounterClockwise (Same as above opposite direction)

  int counterClockwiseSteps = 0;
  int stepInterval = startingSteps;
  int stepIntervalCount = 0;
  bool init = false;
  float rateOfIncrease = 0;
  float oldRateOfIncrease =0;
  //new sum of readings
  float sumofData =0;
  float oldsumofData = 0;

  ReadCurrentSensor();
  avgPower.oldCurrent = avgPower.current;
  avgPower.oldVoltage = avgPower.loadVoltage;
  avgPower.oldPower = avgPower.power;

  while (stepInterval > 2 && counterClockwiseSteps < 1024){
    //Clockwise movement (-1) Counterclockwise movement (1)
    myStepper.step(1);
    //Keep track of local steps
    counterClockwiseSteps++;
    //Keep track of global steps (for untangle cord function)
    posData.totalSteps++;
    //Increment step interval count, for sensor check condition
    stepIntervalCount++;

    //When stepIntervalCount matches the stepInterval, run the sensor check
    if(stepIntervalCount == stepInterval){
      //Reset stepIntervalCount
      stepIntervalCount = 0;
      //Get new readings
      ReadCurrentSensor();
      //Update the sumofData
      sumofData = (avgPower.current+avgPower.loadVoltage+(avgPower.power*2));
      rateOfIncrease = (sumofData - oldsumofData)/stepInterval;

      //check if the loop has initialized
      if(init){
        //The panel is moving and the readings are increasing at a faster rate than before, gradually increase the stepInterval to move the motor a bit more. The next check will be later.
        if(rateOfIncrease >= oldRateOfIncrease){
          stepInterval = stepInterval+addSteps;
        }
        //readings are not increasing at a faster rate
        else{
          if(rateOfIncrease > 0){
            stepInterval = stepInterval;
          }
          else{
            stepInterval = stepInterval/divideSteps;
          }

        }
      }
      //Print log debug
      Serial.print("rate of increase "); Serial.println(rateOfIncrease,6);
      Serial.print("old rate of increase "); Serial.println(oldRateOfIncrease,6);
      Serial.print("sumOfData "); Serial.println(sumofData,6);
      Serial.print("sumOfData Equation "); Serial.println("power: "+ String(avgPower.power) +" + current:"+ String(avgPower.current) +" + Lvoltage"+ String(avgPower.loadVoltage) + "/" + String(stepInterval));
      Serial.print("oldsumofData "); Serial.println(oldsumofData,6);

      //Set the old readings to the new ones for this check
      oldRateOfIncrease = rateOfIncrease;
      oldsumofData = sumofData;
      avgPower.oldCurrent = avgPower.current;
      avgPower.oldVoltage = avgPower.loadVoltage;
      avgPower.oldPower = avgPower.power;
      init = true;
      if(posData.maxReading < sumofData){
        posData.maxReading = sumofData;
      }

      

    }

    
  }
  return counterClockwiseSteps;
}



