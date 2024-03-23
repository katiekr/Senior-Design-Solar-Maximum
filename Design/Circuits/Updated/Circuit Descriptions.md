### Here you will find iterative circuit designs to progress testing our hardware components. These include: ###
- Testing the Current Reader Adafruit INA219 5v, which is the most important component, as it is getting data from the solar panel's current. We need to insure it is sensitive enough to detect changes in current.
- Testing the stepper motor. This is to get an initial positioning algorithm working, and see how different steps correspond to the angle of rotation.
- Testing with final prototype, this tests the charge controller and powers everything without a pc hookup.


>Note: ACS712 5v was tested in the old diagrams, it was not sensitive enough for accurate data.
>Note: steppers replaced servos because of the rotation limits on servos as well as the lack of rotational accuracy.
