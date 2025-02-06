#include <FastLED.h>
#include "FastLED_Plus.h"  //extend FastLED library for SimHum RGB LED Effects

#define NUM_LEDS 18
#define DATA_PIN 6
#define DIMMING_STEPS 8
#define MAX_BRIGHTNESS 100  //maximum brightness [0-255]
#define DEBUG_ENABLED true  //serial output messages
#define REFRESH_RATE 10     //RefreshRate [10 FPS]
#define POT_PIN A1          // Potentiometer input to represent RPMs

unsigned long previousMillis = 0;                                   //last time HID was polled
const long refreshRateDelayInMillis = ((1 / REFRESH_RATE) * 1000);  //HID Polling delay in milliseconds

CRGB leds[NUM_LEDS];  //LED strip as array of CRGB colors for FastLED lib

/*#############################################################################*/
void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();  //all to black
  FastLED.show();   //refresh strip

  RPMGauge RPMGauge_Left = createRPMGauge(
    CRGB * leds,            //pointer to LED strip
    uint16_t 0,             //Starting LED
    uint16_t NUM_LEDS / 2,  //Number of LEDs to include in effect
    CRGB::Green,            //Starting Color in CRGB format
    CRGB::Red,              //Ending color in CRGB format
    uint16_t 0,             //minimum RPM value (Default: 0%)
    uint16_t 100,           //maximum RPM value (Default: 100%)
    bool true,              //fade last LED
    bool true,              //animation direction
    bool true,              //blink Color 1 & 2 when RPM = 100%
    CRGB::Red,              //Color 1 of RPM Redline animation blink
    CRGB::Blue,             //Color 2 of PRM redline animation blink
    uint16_t 50             //blink delay for redline blink animation
  );
}

/*#############################################################################*/
void loop() {
  updateRPMGauge(uint16_t 56, RPMGauge_Left);
}