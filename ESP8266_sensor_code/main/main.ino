//Make sure you set upload speed to 115200
#include <Wire.h> //Communication between DS3231 time module and ESP8266
#include <RtcDS3231.h> //library for DS3231 time module
#include <ArduinoJson.h> //allow for JSON objects
#include <ESP8266WiFi.h> //for Wifi module on MCU
#include <FirebaseArduino.h> //library for Firebase
#include <NTPClient.h> // Libraries to connect to NTP Client ("Time Server")
#include <WiFiUdp.h>  // Libraries to connect to NTP Client ("Time Server")

// Wifi Keys  -  ||Change as needed||
#define WIFI_SSID "INSERT"
#define WIFI_PASSWORD "INSERT"

//Initialization
RtcDS3231<TwoWire> Rtc(Wire); 

//Global Variables
uint32_t initial_time;
uint32_t final_time;
uint32_t flow_duration;
float water_volume;
int sensor_activated = 0;
byte sensorInterrupt = 14;  // 0 = digital pin 2
StaticJsonBuffer<200> buf2;
JsonObject& package2 = buf2.createObject();

//Constants
const float FLOWRATE_THRESHOLD = 0.01;

void setup()
{
  Serial.begin(115200);
  //  Flow rate sensor & time-server connection initialization
  initialize_time_connection();
  initialize_flow_rate_sensor();
  initialize_wifi(); 

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
    send_http_request(initial_time, water_volume);

   }
}
