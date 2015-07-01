# Arduino-BLE-Bridge
This project utilizes the HM-10/11 Bluetooth 4.0 modules to create a serial bridge between Arduino devices.

##About Project:
This project is designed to be a template for you to add your own functionality as you see applicable to your project.


##About the project files:

###centralDevice:
This arduino program trains, or configures, the HM-10 module to act as a bluetooth master device. It handles all connection setup and lets the user type into the serial console (while connected to a PC) and the typed messages will be delivered to the peripheral device.
NOTE: as of version 1.0, the MAC address of the peripheral device needs to be hard-coded into the program. I hope to implement a way to connect to the nearest bluetooth peripheral instead.

###peripheralDevice:
This arduino program trains the HM-10 module to act as peripheral device. This program handles connection, and connection loss (somewhat) gracefully.

###peripheralDeviceEcho:
I this version separate from peripheralDevice because it serves well as an example application. In short, this application sends a message at a user set interval (in terms of iteration count).
