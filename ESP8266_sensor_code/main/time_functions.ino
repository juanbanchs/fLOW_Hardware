void initialize_time_module()
{
  Rtc.Begin();
  // Ignored error catching for now
  // never assume the Rtc was last configured by you, so just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

uint32_t get_time()
{
  RtcDateTime now = Rtc.GetDateTime();
//  Serial.print(now.Epoch32Time()); This would print the time
  return now.Epoch32Time();
}

uint32_t get_duration(uint32_t initial_time, uint32_t final_time)
{
  return final_time - initial_time;
}
