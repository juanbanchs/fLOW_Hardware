/*
 NodeMCU DEVKIT v1.0
 Built-in LED blink
 Caution: This LED is active low.
 Note that this sketch uses LED_BUILTIN to find the pin connected to this LED.
*/

void setup() {
  Serial.begin(115200);             // Communication at 115200 Bd with Serial Monitor
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Setting led on"); // Print a message
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on
  delay(1000);                      // Wait for a second

  Serial.println("Setting led off");// Print a message
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
  delay(2000);                      // Wait for two seconds
}
