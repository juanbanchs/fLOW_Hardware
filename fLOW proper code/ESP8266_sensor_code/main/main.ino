#include <Wire.h> //Communication between DS3231 time module and ESP8266
#include <RtcDS3231.h> //library for DS3231 time module

//Initialization
RtcDS3231<TwoWire> Rtc(Wire); //Good luck?

//Variables
uint32_t initial_time;
uint32_t final_time;
uint32_t flow_duration;
float water_volume;
int sensor_activated = 0;
byte sensorInterrupt = 0;  // 0 = digital pin 2


//Constants
const float FLOWRATE_THRESHOLD = 0.1;

void setup() 
{
  Serial.begin(57600);
  Serial.print("In setup");
  
  //  Flow rate sensor & time module initialization
  initialize_time_module();
  initialize_flow_rate_sensor();
}

void loop() 
{
   attachInterrupt(sensorInterrupt, start_collecting, RISING);
   if (sensor_activated)
   {
      detachInterrupt(sensorInterrupt); //detach the triggering
      initial_time = get_time();
      water_volume = get_vol();
      final_time = get_time();
      flow_duration = get_duration(initial_time, final_time);
      Serial.print("Water volume: ");
      Serial.println(water_volume);
      Serial.print("Duration: ");
      Serial.println(flow_duration);
      Serial.println("left get_vol");

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
