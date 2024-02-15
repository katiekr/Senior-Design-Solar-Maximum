> 2/14/22 
> ## Current module testing
- ACS712 module testing with 220ohm resistor and blue LED (3.5v) showed very little variation in current in indirect sunlight vs indoors (5pm).
  
    The load might need to be increased for more current draw, so the ACS712 current sensor can hopefully detect variations more accurately.
    Further testing might be needed with the charge controller and battery, which is in diagram one but with the servo absent.
  
    Charging the battery should provide sufficient load. **However if the battery is full, the current will drop and the solar panel will stop adjusting for an optimal angle until the battery depletes.
    This might be beneficial to prevent overcharging and increase effeciency, as the panel would only start moving again when the battery starts to deplete.**  

- **The bit resolution of the arduino board might be a limiting factor** using the hall effect from ACS712, which has no onboard processing and requires the user to implement code to convert to mA and is analog to analog at 10bits. This data is processed by the Arduino directly.
    Adafruit INA219 uses a different method for measurement. Through a "shunt resistor" and an **onboard processing resolution of 12bits** to directly output current(mA), voltage(mV), and power(mW). These values are digital on the INA219 board, then directly output to analog to the arduino, **bypassing the     10bit processing limit**. As the arduino is not doing the processing, only recieving processed values from the INA219 board.
  
- **ACS712 has a current resolution of 75mA while the Adafruit INA219 has a resolution of 0.08mA.** So it will probably be switched as the main sensor if testing proves this increase in accuracy.
- A few other issues encountered with the ACS712 module occurred when it was moved or bumped (not the solar panel), and resulted in large current fluctuations. Since the solar panel will be moving on a servo this could pose issues for excessive noise and calibration difficulties.
- To be thorough 4 ACS712 modules were swapped and tested, they all showed similiar issues mentioned above. This rules out manufacturing errors or bad modules.
- I've ordered 4 INA219 modules and will be testing these when they arrive this week.

> ## Solar Charge Controller Testing
- Polarity of the Lipo battery plug did not match the port on the charge controller, so the plug was cut and the wires were stripped to be connected in the onboard two pole connector.
- Interfacing with the arduino on a PC was possible while it was being powered with the charge controller and lipo battery connected.
