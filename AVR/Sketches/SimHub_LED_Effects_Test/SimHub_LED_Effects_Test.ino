#include <FastLED.h>
#include "RPMsEffect.h"  //extend FastLED library for SimHum RGB LED Effects
#include "BlinkEffect.h"

#define NUM_LEDS 18
#define DATA_PIN 6
#define DIMMING_STEPS 8
#define MAX_BRIGHTNESS 10   //maximum brightness [0-255]
#define DEBUG_ENABLED true  //serial output messages
#define REFRESH_RATE 50     //RefreshRate [10 FPS]
#define POT_PIN A1          //Potentiometer input to represent RPMs
#define BUTTON_PIN 10       //Push Button input for testing flags

unsigned long previousMillis = 0;                                   //last time HID was polled
const long refreshRateDelayInMillis = ((1 / REFRESH_RATE) * 1000);  //HID Polling delay in milliseconds

//LED strip as array of CRGB colors for FastLED lib
CRGB leds[NUM_LEDS];


//RPMs effect arguments
//CRGB *leds          //pointer to LED strip
//bool rightToLeft    //animation direction
//uint16_t startPos   //Starting LED
//uint16_t ledCount   //Number of LEDs to include in effect
//CRGB startColor     //Starting Color in CRGB format
//CRGB endColor       //Ending color in CRGB format
//uint16_t minRPM,    //minimum RPM value (Default: 0%)
//uint16_t maxRPM     //maximum RPM value (Default: 100%)
//bool redlineBlink   //blink Color 1 & 2 when RPM = 100%
//CRGB redlineColor1  //Color 1 of RPM Redline animation blink
//CRGB redlineColor2  //Color 2 of PRM redline animation blink
RPMsEffect RPMs_Left(leds, false, 3, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Red, CRGB::Blue);
RPMsEffect RPMs_Right(leds, true, 9, 6, CRGB::Green, CRGB::Red, 0, 100, true, CRGB::Blue, CRGB::Red);

BlinkEffect yellowFlag1(leds, 1, 0, 3, CRGB::Yellow, CRGB::Black);
BlinkEffect yellowFlag2(leds, 1, 15, 3, CRGB::Yellow, CRGB::Black);

/*#############################################################################*/
void setup() {

  //debugger
  if (DEBUG_ENABLED) {
    Serial.begin(115200);
    Serial.println(">>Serial Initialized..");
  }

  //Set Input Modes
  //pinMode(POT_PIN, INPUT_ANALOG);
  pinMode(BUTTON_PIN, INPUT);

  //Initialize the LED Strip using FastLED library
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);  //set maximum brightness for the LEDs
  FastLED.clear();                        //all to black
  FastLED.show();                         //refresh strip

  //[OPTIONAL]Set blink delay (default 100ms)
  RPMs_Left.setBlinkDelay(100);
  RPMs_Right.setBlinkDelay(100);
  yellowFlag1.setBlinkDelay(250);
  yellowFlag2.setBlinkDelay(250);
}

/*#############################################################################*/
void loop() {
  /* Timer */
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= refreshRateDelayInMillis) {
    previousMillis = currentMillis;  //save last time polled.

    //FastLED.clear();

    //Potentiometer to RPM%
    uint16_t potValue = analogRead(POT_PIN);  //for testing

    //lowered pot rangebecause of battery power testing (brownout reduction)
    uint16_t rpmPercentage = map(potValue, 0, 1000, 0, 100);  //to %
    //uint16_t rpmPercentage = map(potValue, 0, 1023, 0, 100);  //to %

    //serial print pot value for debugging
    if (DEBUG_ENABLED) { Serial.println(rpmPercentage); }

    RPMs_Left.update(rpmPercentage);
    RPMs_Right.update(rpmPercentage);

    bool state = digitalRead(BUTTON_PIN);
    yellowFlag1.update(state);
    yellowFlag2.update(state);

    FastLED.show();
  }
}