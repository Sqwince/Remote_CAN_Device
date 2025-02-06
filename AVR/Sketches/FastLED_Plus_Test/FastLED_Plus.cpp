#include "FastLED_Plus.h"

//Constructor
FastLED_Plus::FastLED_Plus() {
  //_leds = leds;
  // _numLeds = numLeds;
}


/* Creates a RPM gauge struct to store parameters
   that resembles SimHub RPM Gauge Effect*/

//returns RPMGauge struct which holds settings for the RPMGauge effect
//TODO: Should this be called 'TACH' instead?
RPMGauge FastLED_Plus::createRPMGauge(
  CRGB* _leds,         //pointer to LED strip
  uint16_t _startPos,  //Starting LED
  uint16_t _ledCount,  //Number of LEDs to include in effect
  CRGB _startColor,    //Starting Color in CRGB format
  CRGB _endColor,      //Ending color in CRGB format
  uint16_t _minRPM,    //minimum RPM value (Default: 0%)
  uint16_t _maxRPM,    //maximum RPM value (Default: 100%)
  bool _useDimming,    //fade last LED
  bool _rightToLeft,   //animation direction
  //TODO: optional include redline flashing with thse optional args?
  CRGB _redlineColor1,     //Color 1 of RPM Redline animation blink
  CRGB _redlineColor2,     //Color 2 of PRM redline animation blink
  uint16_t _blinkDelay) {  //blink delay for redline blink animation

  RPMGauge gauge;  //new instance RPMGauge to return

  gauge.leds = _leds;                    //pointer to LED strip
  gauge.startPos = _startPos;            //Starting LED
  gauge.endPos = _ledCount;              //Number of LEDs to include in effect
  gauge.startColor = _startColor;        //Starting Color in CRGB format
  gauge.endColor = _endColor;            //Ending color in CRGB format
  gauge.minRPM = _minRPM;                //minimum RPM value (Default: 0%)
  gauge.maxRPM = _maxRPM;                //maximum RPM value (Default: 100%)
  gauge.useDimming = _useDimming;        //fade last LED
  gauge.rightToLeft = _rightToLeft;      //animation direction
  gauge.redlineColor1 = _redlineColor1;  //Color 1 of RPM Redline animation blink
  gauge.redlineColor2 = _redlineColor2;  //Color 2 of PRM redline animation blink
  gauge.blinkDelay = _blinkDelay;        //blink delay for redline blink animation

  return gauge;
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
