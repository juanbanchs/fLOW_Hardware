# fLOW_Backend

This repository is for the Arduino code for fLOW, a project to develop a water usage sensor to bring awareness about water waste.

### Utilizing the code

In order to run this code on the ESP8266 Wifi MCU, please make sure you have these set:
* The board selected is 'Node MCU 1.0'. Select this in Tools > Board > scroll down to Node MCU 1.0
  * If 'Node MCU  1.0' is not an option, follow the steps [here](https://www.instructables.com/id/Esp8266-Firebase-Connection/) to install the board manager
* Ensure the libraries are installed by going to Sketch > Include Library > Manage Libraries
  * ArduinoJson 5.13.1

### Folders

#### Libraries

Ensure that all libraries in the 'Libraries' folder are imported into the Arduino Library. 
Please note that ArduinoJson should be version 5.13.1 because the firebase-arduino library is not compatible with ArduinoJson 6.

#### setting_time

This contains a piece of Arduino code used to set the time module (DS3231) in case the battery comes out and the modeul resets.
To the best of our knowledge, it should be run on an Arduino Uno

#### ESP8266_sensor_code

We have modularized our code involving collecting data from the flow rate sensor and time module and pushing to Firebase
