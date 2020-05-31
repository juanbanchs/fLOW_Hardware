void initialize_firebase()
{
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void push_to_firebase(JsonVariant data_package)
{
  //Push a log
//  Firebase.push("data", data_package);
  String path = "water_data/" + String(initial_time);
  Firebase.set(path, data_package);
  // handle error
  if (Firebase.failed()) {
      Serial.print("Pushing Data Package Failed:");
      Serial.println(Firebase.error());  
//      ESP.reset();
      return;
  }
}
