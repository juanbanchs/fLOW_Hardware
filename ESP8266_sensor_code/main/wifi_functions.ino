void initialize_wifi()
{
  // Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
  // would try to act as both a client and an access-point and could cause 
  // network-issues with your other WiFi-devices on your WiFi-network.

  // Uncomment the Serial.print statements to debug wifi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected to Wifi. local IP Address: ");
  Serial.println(WiFi.localIP());
}
