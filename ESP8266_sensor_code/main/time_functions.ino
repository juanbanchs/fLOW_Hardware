// Define NTP Client to get time
const long utcOffsetInSeconds = -25200; //From Central time to PT (Los Angeles) time
WiFiUDP ntpUDP; //Using User Datagram Protocol (ETBD)
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void initialize_time_connection()
{
  timeClient.begin();
}

uint32_t get_time()
{
  timeClient.update();
//  Serial.print(timeClient.getEpochTime()); //This would print the time
  return timeClient.getEpochTime();
}

uint32_t get_duration(uint32_t initial_time, uint32_t final_time)
{
  return final_time - initial_time;
}


///////////////// Below are the previous functions used for the Time Module /////////////////////

//void initialize_time_module()
//{
//  Rtc.Begin();
//  // Ignored error catching for now
//  // never assume the Rtc was last configured by you, so just clear them to your needed state
//  Rtc.Enable32kHzPin(false);
//  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
//}
//
//uint32_t get_time()
//{
//  RtcDateTime now = Rtc.GetDateTime();
////  Serial.print(now.Epoch32Time()); This would print the time
//  return now.Epoch32Time();
//}
