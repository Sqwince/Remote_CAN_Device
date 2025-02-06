#include <FastLED.h>
#include "SH_Effects.h"  //extend FastLED library for SimHum RGB LED Effects

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

s_RPMs RPMs_Left;
s_RPMs RPMs_Right;

/*#############################################################################*/
void setup() {
  //Initialize the LED Strip using FastLED library
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.maximizeBrightness(100); //set maximum brightness for the LEDs
  FastLED.clear();  //all to black
  FastLED.show();   //refresh strip

  //RPMs effect using left half of LED Strip
  s_RPMs RPMs_Left = createRPMsEffect(
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
    uint16_t 100             //blink delay for redline blink animation
  );

  //RPMs effect using left half of LED Strip
  s_RPMs RPMs_Reft = createRPMsEffect(
    CRGB * leds,            //pointer to LED strip
    uint16_t ((NUM_LEDS / 2)+1, //Starting LED
    uint16_t NUM_LEDS / 2,  //Number of LEDs to include in effect
    CRGB::Green,            //Starting Color in CRGB format
    CRGB::Red,              //Ending color in CRGB format
    uint16_t 0,             //minimum RPM value (Default: 0%)
    uint16_t 100,           //maximum RPM value (Default: 100%)
    bool true,              //fade last LED
    bool false,              //animation direction
    bool true,              //blink Color 1 & 2 when RPM = 100%
    CRGB::Red,              //Color 1 of RPM Redline animation blink
    CRGB::Blue,             //Color 2 of PRM redline animation blink
    uint16_t 100             //blink delay for redline blink animation
  );
}

/*#############################################################################*/
void loop() {

  uint16_t potValue = analogRead(POT_PIN); //for testing
  uint16_t rpmPercentage = map(potValue, 0, 1023, 0, 100); //to %

  updateRPMGauge(rpmPercentage, RPMs_Left);
}