const int analogInPin = A0; // Analog input pin that the sensor is attached to
const float sensorVoltage = 5.000; // Supply voltage to the ACS712
const float sensitivity = 0.185; // Sensitivity in V/A for the 5A sensor version
const float offsetVoltage = sensorVoltage / 2; // Zero current output voltage
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  unsigned int x=0;
  float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

  for (int x = 0; x < 150; x++){ //Get 150 samples
    AcsValue = analogRead(A0);     //Read current sensor values   
    Samples = Samples + AcsValue;  //Add samples together
    delay (3); // let ADC settle before next sample 3ms

  }
  AvgAcs=Samples/150.000;//Taking Average of Samples
  // put your main code here, to run repeatedly:



  // Convert the voltage to current using the sensor sensitivity
  // Subtract the offset voltage (voltage at zero current)
  // and then divide by the sensitivity
  float current = (offsetVoltage-(AvgAcs*(sensorVoltage/1023.0))) / sensitivity;

  // Print the current value to the Serial Monitor:
  //Serial.print("Current: ");
  //Serial.print(current);
  //Serial.println(" A");

  //Serial.println(current);
  Serial.println(AvgAcs,5);
  delay(50);

}
