#include <FastLED.h>
#include "RPMsEffect.h"  //extend FastLED library for SimHum RGB LED Effects

#define NUM_LEDS 18
#define DATA_PIN 6
#define DIMMING_STEPS 8
#define MAX_BRIGHTNESS 100  //maximum brightness [0-255]
#define DEBUG_ENABLED true  //serial output messages
#define REFRESH_RATE 10     //RefreshRate [10 FPS]
#define POT_PIN A1          // Potentiometer input to represent RPMs

unsigned long previousMillis = 0;                                   //last time HID was polled
const long refreshRateDelayInMillis = ((1 / REFRESH_RATE) * 1000);  //HID Polling delay in milliseconds

//LED strip as array of CRGB colors for FastLED lib
CRGB leds[NUM_LEDS];
//RPMs effect using left half of LED Strip
RPMsEffect RPMs_Left(leds, false, 0, NUM_LEDS / 2, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Yellow, CRGB::Blue);

/*#############################################################################*/
void setup() {
  //Initialize the LED Strip using FastLED library
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);  //set maximum brightness for the LEDs
  FastLED.clear();             //all to black
  FastLED.show();              //refresh strip

  //RPMs effect using right half of LED Strip
  //RPMs effect using left half of LED Strip
  // RPMsEffect RPMs_Right(
  //   leds,            //pointer to LED strip
  //   true,              //left to right ordering
  //   uint16_t NUM_LEDS,      //Starting LED
  //   uint16_t NUM_LEDS / 2,  //Number of LEDs to include in effect
  //   CRGB::Green,            //Starting Color in CRGB format
  //   CRGB::Red,              //Ending color in CRGB format
  //   uint16_t 0,             //minimum RPM value (Default: 0%)
  //   uint16_t 100,           //maximum RPM value (Default: 100%)
  //   bool true,              //blink Color 1 & 2 when RPM = 100%
  //   CRGB::Yellow,           //Color 1 of RPM Redline animation blink
  //   CRGB::Black);           //Color 2 of PRM redline animation blink
}

/*#############################################################################*/
void loop() {
  /* Timer */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= refreshRateDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.

    //Potentiometer to RPM%
    uint16_t potValue = analogRead(POT_PIN);                  //for testing
    uint16_t rpmPercentage = map(potValue, 0, 1023, 0, 100);  //to %

    RPMs_Left.update(rpmPercentage);
    //RPMs_Right.update(rpmPercentage);
    FastLED.show();  //refresh strip
  }
}