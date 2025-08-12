/*

  * This example shows how to use the ESP32's deep sleep mode.
  * The ESP32 will wake up every 5 seconds, blink the built-in LED,
  * and then go back to sleep.
  * The boot count is stored in RTC memory so that it can be retained across deep sleep cycles.

*/

#include <Arduino.h>

//PIN 8 is the default pin for the ESP32 built-in LED (for ESP32 boards that have a built-in LED)
#define LED_BUILTIN 8

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5          /* Time ESP32 will be asleep (in seconds) */

// The boot count is stored in RTC memory so that it can be retained across deep sleep cycles
//RTC_DATA_ATTR is used to place the variable in RTC memory
//Use RTC memory for variables that need to be preserved across deep sleep cycles
RTC_DATA_ATTR int bootCount = 0;

long awakeTime = 5000; // Time the ESP32 should stay awake in miliseconds

void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize Serial for debugging
  Serial.begin(9600);
  delay(1000);  //Take some time to open up the Serial Monitor

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  /*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */

  //esp_sleep_enable_timer_wakeup is used to set the timer wakeup source
  //Please see the docs for other wakeup sources
  //https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/sleep_modes.html#timer-wakeup
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

}

void loop() 
{
  // Blink the LED to indicate that the ESP32 is awake
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                      // wait for half a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(500);                      // wait for half a second

  //The program will be kept awake for 5 seconds
  long currentMillis = millis();
  if (currentMillis >= awakeTime) 
  {
    Serial.println("Going to sleep now");
    //Waits for the transmission of outgoing serial data to complete.
    Serial.flush();
    //Go to deep sleep
    esp_deep_sleep_start();
  }
}