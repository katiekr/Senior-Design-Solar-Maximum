
# Hardware Setup

## Components needed
- DC 5V to DC 9V USB Voltage Step Up Converter Cable 
- 28BYJ-48 Stepper Motor and motor control board
- ELGATO UNO R3 Compatible with Arduino IDE or an Arduino R3 equivalent board
- Sunnytech 0.5w 5v 100ma Mini Small Solar Panel Module DIY Polysilicon Solar Epoxy Cell Charger B016 or an equivalent 0.5w 5v 100ma solar panel
- INA219 adafruit Module Compatible for Arduino
- 3.7V Lithium ion Battery 1100mAh 603449 Rechargeable 3.7 Volt Lipo Battery Pack or an equivalent battery pack
- 900mA MPPT Solar Panel Controller
- Wiring for connecting components
- Male to male Usb connector for the Arduino board (for installing software and connecting the charge controller) This should come with the Arduino board
- A computer or laptop than can run Arduino IDE
- Access to a 3D printer to mount the solar panel to the stepper motor. (You can improvise and make your own if you don't have access to a 3D printer)
- A mounting platform such as a piece of wood and wood screws

![Finished Build](https://github.com/cabledc/Senior-Design-Solar-Maximum/blob/main/Design/Final_ProtoType.jpg?raw=true)



 
## Circuit Setup
The circuit below represents the final prototype wiring. When installing the software plug the arduino into the pc through the usb port. When finished, disconnect from the PC/Laptop and make sure the charge controller is plugged into the 9v port in the arduino. 

Note: You can keep the laptop plugged in for debugging if needed. Some components such as the INA219 adafruit Module might need to be soldered together, this depends on where you order the part. 

If no data is being printed to your debug logs, your current sensor likely isn't working, or connected correctly. A debug log should appear "Failed to find INA219 chip" if this is the case.


![Circuit](https://github.com/cabledc/Senior-Design-Solar-Maximum/blob/main/Design/Circuits/Updated/Final%20Diagram/Final_Pic.PNG?raw=true)



>[Return](https://github.com/cabledc/Senior-Design-Solar-Maximum/tree/main?tab=readme-ov-file#user-documentation)
