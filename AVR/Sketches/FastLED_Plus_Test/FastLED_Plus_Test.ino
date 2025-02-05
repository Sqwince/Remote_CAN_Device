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

//WS2812B,GRB, LED_PIN, NUM_LEDS
FastLED_Plus effects();



/*#############################################################################*/
void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
}

/*#############################################################################*/
void loop() {
  effects.breathe(leds, NUM_LEDS, 5);
  delay(30);
}