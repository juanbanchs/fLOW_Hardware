#include <ArduinoJson.h>
StaticJsonDocument<200> doc;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  
  JsonObject object = doc.to<JsonObject>();

  JsonObject timestamp = object.createNestedObject("123456"); //inside argument, put the value you want for timestamp
  
  timestamp["duration"] = 10; //put value 
  timestamp["volume"] = 30;

  serializeJsonPretty(object, Serial);
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
