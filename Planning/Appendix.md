# (Senior Design Part 1)
## What has been done:
  ### Hardware
  All the correct hardware has been acquired and the circuits have been designed. The hardware has been researched to match the specs of our design, (voltage, wattage, etc).
- We have found we needed a charge controller instead of connecting a battery directly to the solar panel. Otherwise this would have damaged the battery.
- Stepper motors were switched for servo motors. As stepper motors require constant charge, even when not moving. They also each require a control board making the circuit more complicated.
- Powering arduino components from the charge controller, instead of the arduino directly. Servo motors can spike in amps and potentially cut power from the arduino. Connecting everything to the main power supply (charge controller) prevents this.
- The Current reader applies zero load, so the initial circuit would have shorted and damaged the panel. A 220ohm resistor + LED was added for a load to prevent this.

  ### Software
- We have found supporting libraries exist for the arduino components we are using. These can be imported to allow faster interfacing with the components
  - Research on PID implementation. This will smooth jumpy/uneven output from an algorithm that is adjusting servo position(s).


### What still needs to be done next semester (In Order):
- Hardware prototype assembly.
- Arduino software implementation. And initial component interface testing.
- Each circuit (already designed) will be tested. There are 3, each one is more complex, but allows for a step by step process for testing. Leading to the final circuit.
- 3D printing of housing for the prototypes, if needed.

# (Senior Design Part 2)
### Hardware
After testing servo motors, we realized they cannot do a full 360 degree rotation. There was also no way to keep an accurate measurement of their movement. So we had to switch back to using a stepper motor instead. This allows us to keep track of steps and store information on energy recieved in each direction. As well as providing info for an untangle function.

We switched to Adafruit INA219 for current measurement as it provided higher resolution data, and allowed the device to work indoors with a flashlight. The ACS712 module was not sensitive enough for the low voltages we were using. More info can be found in the testing log summary [Current Testing](https://github.com/cabledc/Senior-Design-Solar-Maximum/blob/main/Testing%20Logs/Testing%20Logs%202-14-22.md)

The circuit diagrams had to be updated as well and can be found here [Updated Circuits](https://github.com/cabledc/Senior-Design-Solar-Maximum/tree/main/Design/Circuits/Updated)

A 3D printed holder for the solar panel and stepper motor mount was created. Those files can be found here [3D Prints](https://github.com/cabledc/Senior-Design-Solar-Maximum/tree/main/3D%20Printer%20Files)

### Software
3 Versions of the software are provided along with the testing for the ACS712 module. Each tracking code release improves on the previous version. The algorithm does not use a PID controller as this didn't quite fit the data and scenario we were trying to use it for. There are 3 main variables that control the new algorithm:
- int addSteps = 2;
  - Positive feedback when the rate of increase is greater than the previous rate of increase, adds steps to move the panel further in its current direction
- int divideSteps = 4;
  - Negative feedback when the rate of increase is less than the previous rate of increase, divides steps to slow the panels movement and increase measurement frequency. Once this is less than 2 the panel stops.
- int startingSteps = 40;
  - Initial movement check, in each direction. A full rotation is 2048 steps.


---
 
#### Overall Listed Objectives Summmary:
- General design and project outline(October)
- Hardware planning and research(November)
- Circuit Design (December)
- Build first test prototype(February)
- Interface components with Arduino IDE(February)
- Mechanical Prototype Development/Testing(End of February)
  - Testing sensors, software calibration and filtering of raw data from the sensors.
  - Ensuring all components are tested and operate correctly and accurately
- Arduino Software Development(March)
  - Use the filtered sensor data to develop code to move the stepper with the attached solar panel

#### Work Division (Second Semester)
Destry Cable
- Prototype Devlopment
  - 3D Printing
  - Circuit Redesign
  - Code Development
  - Testing
  - Soldering
    
Katherine Kraus
- Testing Planning
  
Alexander Miller
- Logo and Poster Design

    
