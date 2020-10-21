# fLOW_Hardware
Before you go any further, check out this video of this device in action! [http://bit.ly/UCLAfLOW]

This repository is fLOW's Arduino code, a project to develop a water usage sensor to bring awareness about water waste. For more information on this project, please checkout our main repository [here](https://github.com/allisonchen23/fLOW_frontend).

Our hardware consists of the following components:
* An [ESP8266 Wifi MicroConroller Unit (MCU)](https://www.amazon.com/HiLetgo-Internet-Development-Wireless-Micropython/dp/B081CSJV2V/ref=sr_1_1_sspa?dchild=1&keywords=esp8266&qid=1601678227&sr=8-1-spons&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyWjNWR1ozQUFDOFVTJmVuY3J5cHRlZElkPUEwNTk4MzE5MkFYUTVFUzBOMVRZVCZlbmNyeXB0ZWRBZElkPUEwNzkyNTgyMzdMMEZQN0Y2WjBPSCZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1)
* A [flowrate sensor](https://www.amazon.com/DIGITEN-Effect-Sensor-Flowmeter-Counter/dp/B00VKAT7EE/ref=sr_1_5?dchild=1&keywords=flow%20rate%20sensor&qid=1600656372&sr=8-5), or like our [previous one](https://www.amazon.com/uxcell-Effect-Flowmeter-0-2-2-5L-SEN-HZ41W/dp/B07DM2FD51/ref=sr_1_14?keywords=Water+Hall+Effect+Flow+Sensor&qid=1579555540&sr=8-14)
* A 3D printed casing to hold all our parts and attach to the sink. Get your own [here](https://drive.google.com/drive/folders/1zjI_3nG_oABBmjKBfur1F099fGafXyzb?usp=sharing)!
* A [Silicone Sealant](https://www.amazon.com/GE-GE284-Silicone-Kitchen-Squeeze/dp/B000BQWKCY/ref=sr_1_5?dchild=1&keywords=100%25%2Bsilicone%2Bsealant&qid=1587936819&sr=8-5&th=1) to secure fit onto your sink

### Utilizing the code

Prior to working with this code, make sure you download the Arduino IDE [here](https://www.arduino.cc/en/main/software).

First clone the repository:
1. Click the green 'Code' button and copy the link to clone.
2. Use Github Desktop or a terminal (Command Prompt for Windows) to navigate to your desired directory
3. Use the command `git clone <paste the link here>`

Change your device ID and Wifi information by navigating to the cloned repository and find the file **ESP8266_sensor_code/main/main.ino**. In lines 13 and 14, set the Wifi name and password to your own Router's name and password. Then navigate to the file **ESP8266_sensor_code/main/http_functions.ino**. At the top, change the variable `device_ID` to a unique integer. (It's not ideal, but you can check which numbers are already taken by going to [this link](http://allisonchen.co/fLOW_frontend/#/fLOW_frontend/pages/about.js), click on the groups icon on the left panel, and click on 'Select' to see which numbers are taken. Please note that the variable `device_ID` is an INTEGER, not a string.)

Arduinos can be finicky, so in order to help you use this code for your own devices, make sure you do the following: 
In order to run this code on the ESP8266 Wifi MCU, please make sure you have these set:
* The board selected is 'Node MCU 1.0'. Select this in Tools > Board > scroll down to Node MCU 1.0
  * If 'Node MCU  1.0' is not an option, follow the steps [here](https://www.instructables.com/id/Esp8266-Firebase-Connection/) to install the board manager
* Ensure the libraries are installed by going to Sketch > Include Library > Manage Libraries
  * ArduinoJson by Benoit Blanchon Version 5.13.2
  * NTPClient by Fabrice Weinberg 3.2.0 Help [here](https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/)
  
Now if you plug in your device, ensure your baud rate and ports are correct [We used 115200], you should be able to upload your code! Unfortunately, it currently does not work if not plugged into a computer, but that is (hopefully) an enhancement for the future!

The first thing you should probably do is calibrate your sensor, this is quite a simple feat:
1. Firstly after successfully uploading the code, open the "Serial Monitor" and use a measuring cup to measure around 500 ml of water.
2. Pour this through the flowrate sensor (or inversely, pour water through your flowrate sensor into a measuring cup until around 500ml of water is used).
3. Note down the measured value on the serial monitor.
4. In the sensor_functions (**ESP8266_sensor_code/main/sensor_functions.ino**), look for the calibrationFactor and note down the value.
5. Take the actual volume (500 ml) divided by the measured volume (e.g. 400 ml), and multiply this by the calibrationFactor to obtain your new calibration factor, basically:
6. NewCalibrationFactor = (ActualVol/MeasuredVol)*OldCalibrationFactor

### How Data is Collected

Our device currently is made to attach to the end of the sink. Although we've found that *most* home sinks are approximately the same diameter, we utilize a tightening mechanism similar to collets in machine shops. The collet is cylindrical with slits to allow flexibility if pressure is applied radially. The device is attached to the end of the sink by screwing it on which squeezes the collet to tightly hug the end of the sink. 

Once the code is uploaded to the device, the device is ready to be used! Simply run some water through the flow rate sensor and on the Arduino console, you should be able to see how much water is recorded. If you navigate to the [website](http://allisonchen.co/fLOW_frontend/#/fLOW_frontend/pages/about.js), click on the groups page on the left panel, and select your unique device ID, you should see data!

On a high level, here's how our collection works. When water runs through the flow rate sensor, the Arduino first logs a timestamp, by sending an HTTP request. Then, it accumulates the total quantity essentially by multiplying rate by time. Once the flow of water stops, the MCU packages the volume along with the device ID and a timestamp of the collection, ties a red bow on top, and sends a request to the backend server to update the database. From there, the server recognizes the request and adds an entry into our Postgres database.

### Our Journey

While our hardware is far from perfect, it certainly is a big step up from where we started: using a normal Arduino, a wifi module, a time module (now converted to using the NCP time server), and a flow rate sensor. By using an ESP8266 Wifi enabled MCU, we were able to reduce these 4 components into 2. Originally, we also faced a lot of issues connecting our Wifi module to Firebase, our original database before we used Postgres. There are some Firebase Arduino libraries that we utilized, but due to a change in the Firebase API, most of the tutorials we were utilizing were out of date and would not let us upload data. The solution ended up being found [here](https://medium.com/@o.lourme/our-iot-journey-through-esp8266-firebase-angular-and-plotly-js-part-1-a07db495ac5f) (search "FirebaseHttpClient.h"), something we had found in the depths of the Internet.

Another CS best practice we did our best to implement in our hardware code was utilizing functions. Although we could've had all our files in one main.ino file, by breaking it up into different files depending on what functionality the code had, we were able to not only write cleaner code, but also implement changes more easily when we switched to communicating to a server using HTTP requests directly, not a Firebase API. Huge shoutout to Juan Banchs, one of the mechanical engineers on our hardware team who was mainly in charge of coding the hardware, despite not taking any CS classes before! (Note: Juan did not write this README so he is not giving himself a shoutout here, he would never). 

Upon switching to updating the Postgres database via the server, we weren't too familiar with the types of requests that we could use in HTTP and since the amount of data we are sending is relatively small, we decided to simply use GET requests and the URL to format and submit our data to the server. Looking back, it's probably not the best practice, and a future enhancement could include changing this. 

### Conclusion

It's not perfect, and far from it, but we've also gone a long way from the start of this project! Now, we have an MVP of an IoT device to track personal water usage, and possibly more importantly, we all learned a lot, both technically with working with IoT and just about working with others and in teams. If you made it this far, thanks for reading this README!
