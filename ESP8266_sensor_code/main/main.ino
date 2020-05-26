//Make sure you set upload speed to 115200
#include <Wire.h> //Communication between DS3231 time module and ESP8266
#include <RtcDS3231.h> //library for DS3231 time module
#include <ArduinoJson.h> //allow for JSON objects
#include <ESP8266WiFi.h> //for Wifi module on MCU
#include <FirebaseArduino.h> //library for Firebase

// Firebase and Wifi Keys
//testproj-81eb3.firebaseio.com
//#define FIREBASE_HOST "wifi-test-4f471.firebaseio.com"
//#define FIREBASE_AUTH "cdcAVI94Alk9mgeGwLe10HWI241hnobn26ndzsGQ"
#define FIREBASE_HOST "testproj-81eb3.firebaseio.com/"
#define FIREBASE_AUTH "MsuTT4QhkfwMGufBHZFKGgDbXk5AHTqGKJKQ7RCR"
// Change as needed
//#define WIFI_SSID "Kinkin2"
//#define WIFI_PASSWORD "zanyoctopus986"
#define WIFI_SSID "Singapore"
#define WIFI_PASSWORD "REMFVREMVJSAM"

//Initialization
RtcDS3231<TwoWire> Rtc(Wire); //Good luck?

//Global Variables
uint32_t initial_time;
uint32_t final_time;
uint32_t flow_duration;
float water_volume;
int sensor_activated = 0;
byte sensorInterrupt = 14;  // 0 = digital pin 2
//StaticJsonBuffer<200> buf;
//JsonObject& package = buf.createObject();
StaticJsonBuffer<200> buf2;
JsonObject& package2 = buf2.createObject();

//Constants
// changed from 0.1 to 0.01
const float FLOWRATE_THRESHOLD = 0.01;

void setup() 
{
  Serial.begin(115200);
  Serial.print("In setup");
  
  //  Flow rate sensor & time module initialization
  initialize_time_module();
  initialize_flow_rate_sensor();
  initialize_wifi (); 
  package2["key"] = 2;
}

void loop() 
{
   attachInterrupt(digitalPinToInterrupt(sensorInterrupt), start_collecting, RISING);
   if (sensor_activated)
   {
      detachInterrupt(digitalPinToInterrupt(sensorInterrupt)); //detach the triggering
      initial_time = get_time();
      water_volume = get_vol();
      final_time = get_time();
      flow_duration = get_duration(initial_time, final_time);
      Serial.print("Water volume: ");
      Serial.println(water_volume);
      Serial.print("Duration: ");
      Serial.println(flow_duration);

      //create json package to send to firebase
      if (water_volume > 0)
      {
          StaticJsonBuffer<200> buf;
          JsonObject& package = buf.createObject();
//        JsonObject data = package.createNestedObject(String(initial_time));
//        data["duration"] = flow_duration; //put value 
//        data["volume"] = water_volume;
//        serializeJsonPretty(package, Serial);
//        JsonVariant data_package = create_json_package();
//        Serial.println(data_package.as<String>());
        // Send package
//        JsonVariant data_package = package2;
        create_json_package(package);
        push_to_firebase(package);
        // Clear json document for next round of data  
        buf.clear();
      }
//      Serial.println("left get_vol");

   }
  /*
  //If flowrate > 0.2 L
//  if(sensor_activated == 1)
//  {
    Serial.print("before cur vol");
    int cur_volume = get_water_volume();
    initial_time = get_time();
    Serial.print(cur_volume);
//water_volume = get_water_volume();
//  delay(100);
    final_time = get_time();
    flow_duration = get_duration(initial_time, final_time);
//  Serial.println(flow_duration);
//  Create packet
//  Send to firebase
    sensor_activated = 0;
    */
//  } 
}
