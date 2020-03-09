#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

void setup()
{
  Serial.begin(57600);
  set_time_arduino();
}

void loop()
{
    // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  // Send time
  Serial.println(rtc.getTimeStr());

  delay (1000);
}

void set_time_arduino() 
{
  rtc.begin();
//  rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(6, 25, 40);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(6, 3, 2020);   // Set the date to January 1st, 2014
}
