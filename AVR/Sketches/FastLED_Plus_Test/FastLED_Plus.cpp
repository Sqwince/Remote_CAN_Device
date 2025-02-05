#include "FastLED_Plus.h"

FastLED_Plus::FastLED_Plus() {
  _leds = leds;
  _numLeds = numLeds;
}


/* Creates a RPM gauge that resembles SimHub RPM Gauge Effect*/
void FastLED_Plus::drawRPMGauge(
  CRGB* leds,         //pointer to LED strip
  uint16_t startPos,  //Starting LED
  uint16_t ledCount,  //Number of LEDs to include in effect
  CRGB startColor,    //Starting Color in CRGB format
  CRGB endColor,      //Ending color in CRGB format
  uint16_t minRPM,    //minimum RPM value (Default: 0%)
  uint16_t maxRPM,    //maximum RPM value (Default: 100%)
  bool useDimming,    //fade last LED
  bool rightToLeft,   //animation direction
  //TODO: optional include redline flashing with thse optional args?
  CRGB redlineColor1,    //Color 1 of RPM Redline animation blink
  CRGB redlineColor2,    //Color 2 of PRM redline animation blink
  uint16_t blinkDelay){  //blink delay for redline blink animation



  }




void FastLED_Plus::updateRPMGauge(uint16_t RPMs);








//Un-tested
void FastLED_Plus::breathe(CRGB* leds, uint16_t numLeds, uint8_t speed) {
  static uint8_t brightness = 0;
  static int8_t direction = 1;

  brightness += direction * speed;
  if (brightness == 0 || brightness == 255) {
    direction = -direction;
  }

  for (uint16_t i = 0; i < numLeds; i++) {
    leds[i].fadeToBlackBy(255 - brightness);
  }
  FastLED.show();
}

//Un-tested
void FastLED_Plus::chase(CRGB* leds, uint16_t numLeds, uint8_t color, uint8_t speed) {
  static uint16_t index = 0;
  index = (index + speed) % numLeds;

  fill_solid(leds, numLeds, CRGB::Black);
  leds[index] = CHSV(color, 255, 255);

  FastLED.show();
}
