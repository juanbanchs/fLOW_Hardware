//flow rate sensor variables
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
float calibrationFactor = 4.5;
volatile byte pulseCount = 0; 
float flow_rate = 0.0;
unsigned int flow_liters = 0;
unsigned long total_liters = 0;
unsigned long oldTime = 0;

//flow rate constants
const float SAMPLING_INTERVAL = 1000.0;

int get_water_volume()
{
  oldTime = millis();
  total_liters = 0;

  while (flow_rate >= FLOWRATE_THRESHOLD)
  {
   if((millis() - oldTime) > SAMPLING_INTERVAL)
    {
      // Disable the interrupt while calculating flowrate
      detachInterrupt(sensorInterrupt);
      
      // Calculate flow_rate with scaling for duration & reset oldtime.
      flow_rate = ((SAMPLING_INTERVAL/ (millis() - oldTime)) * pulseCount) / calibrationFactor;
      oldTime = millis();
      
  //  Calculate flowrate (SAMPLING_INTERVAL/1000) accounts for the length of our sample.
      flow_liters = (flow_rate / 60) * (SAMPLING_INTERVAL/1000);
      total_liters += flow_liters;
      print_volume();
      Serial.print("in flowrate loop");
  
        // Reset the pulse counter so we can start incrementing again
      pulseCount = 0;    
      // Enable the interrupt again now that we've finished sending output
      attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
  }  
}

ICACHE_RAM_ATTR void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
  sensor_activated = 1;
  //--------------------------------------------Have to set interrupt to get this done
}

void initialize_flow_rate_sensor()
{
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

//The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
//Configured to trigger on a FALLING state change (transition from HIGH state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

int get_flow_rate()
{
  int current_flow_rate = 0;

  
}

void print_volume()
{
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flow_rate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(total_liters);
    Serial.println("L"); 
}
