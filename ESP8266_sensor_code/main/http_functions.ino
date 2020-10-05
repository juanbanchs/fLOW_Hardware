const char* host = "129.146.137.152";
const int httpPort = 3000;
const int device_ID = 0;

// If you wanted to create the connection from the start.
//void initialize_TCP_connection()
//{
//  delay(5000); 
//  Serial.print("connecting to ");
//  Serial.println(host); // Use WiFiClient class to create TCP connections
//  WiFiClient client;
//
//  if (!client.connect(host, httpPort)) 
//  {
//    Serial.println("connection failed");
//    return;
//  }
//}

//uint32_t get_duration(uint32_t initial_time, uint32_t final_time)
//{
//  return final_time - initial_time;
//}

void send_http_request(uint32_t initial_time, float water_volume)
{
  WiFiClient client;
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URL for the request with informtation we want to send to the server
  String url = "/";
  url += "add/";
  url += initial_time; url += "/";
  url += device_ID; url += "/"; //Here we set the ID
  url += water_volume; url += "/";  
 
  Serial.println(); Serial.print("Requesting URL: "); Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000)
    { Serial.println(">>> Client Timeout !");
      client.stop(); return;
    }
  } // Read all the lines of the reply from server and print them to Serial
  while (client.available())
  { String line = client.readStringUntil('\r'); Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}
