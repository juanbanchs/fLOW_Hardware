#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <DS3231.h>

byte statusLed    = 13; // probs for an led if we want to attach one

byte sensorInterrupt = 0;  // 0 = digital pin 2, used for starting and stopping measuring
byte sensorPin       = 2; // attach sensor to pin 2 on board

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow. 
float calibrationFactor = 100; // this factor needs to be verified for the sensors we are using, used to calculate flow rate as a function of time  

volatile byte pulseCount; // ??

// what we use to calculate the amount of volume passing through the sensor; depends on calibrationFactor
float flowRate; 
// flowing variables are not assigned but are initiated in the setup function 
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

bool ifSend = false;


// Set these to run example.
#define FIREBASE_HOST "wifi-test-4f471.firebaseio.com"
#define FIREBASE_AUTH "cdcAVI94Alk9mgeGwLe10HWI241hnobn26ndzsGQ"
#define WIFI_SSID "UCLA_WEB"
#define WIFI_PASSWORD ""

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  rtc.begin();

   // Initialize a serial connection for reporting values to the host
   
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  StaticJsonDocument<200> doc;
  JsonObject object = doc.to<JsonObject>();
}

int n = 0;

void loop() {
  callSensor(); //call the sensor
  setJson();  //set Json values from sensor readings, time readings
  uploadToFirebase(); //upload to Firebase
}

void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}

void callSensor()
{
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    Serial.print("\t");       // Print tab space
  Serial.print(totalMilliLitres/1000);
  Serial.print("L");
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

void setJson()
{
  if (totalMilliLitres < 10)
  {
    ifSend = false;
    return;
  }
  JsonObject timestamp = object.createNestedObject("123456"); //inside argument, put the value you want for timestamp
  timestamp["duration"] = 10; //put value 
  timestamp["volume"] = totalMilliLitres;
  ifSend = true;
}

void uploadToFirebase()
{
  if (ifSend == false)
  {
    return;
  }
  Firebase.push("arduino_push",timestamp);  //currently, path is arduino_push for testing
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      ESP.reset();
      return;
  }
}
