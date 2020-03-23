void create_json_package(JsonObject& package)
{
//  JsonObject& data = package.createNestedObject(String(initial_time));
//  data["duration"] = flow_duration; //put value 
//  data["volume"] = water_volume;
  package["duration"] = flow_duration; //put value 
  package["volume"] = water_volume;
  String ret;
//  JsonVariant package_var = package; //need to wrap JsonObject 
//  ret = package.as<String>();
  package.printTo(ret);
  Serial.println(ret);
//  return package_var;

}
